import java.io.*;
import java.util.*;
import java.util.regex.*;

public class MacroPass1 {

    // ---- Table entry classes ----
    static class MNTEntry {
        String name;
        int mdtIndex;   // MDTP at start of this macro
        int kpdtIndex;  // KPDTP (0 if none)
        int pntabIndex; // start index in PNTAB (1-based)
        int ppCount;    // number of positional/keyword params total
        int kpCount;    // number of keyword params
        int sstp;       // start in SSTAB (not fully used here)
        List<String> paramList = new ArrayList<>(); // ordered param names
        List<String> evList = new ArrayList<>();    // expansion vars
        public String toString() {
            return String.format("%s  MDT:%d  KPDTP:%d  PNTAB:%d  #PP:%d  #KP:%d  SSTP:%d",
                    name, mdtIndex, kpdtIndex, pntabIndex, ppCount, kpCount, sstp);
        }
    }

    static class KPDTEntry {
        String name;
        String defaultVal;
        public KPDTEntry(String n, String d) { name = n; defaultVal = d; }
        public String toString() { return name + " = " + defaultVal; }
    }

    static class PNTEntry {
        String macroName;
        String param;
        public PNTEntry(String m, String p) { macroName = m; param = p; }
        public String toString() { return macroName + " : " + param; }
    }

    static class EVEntry {
        String macroName;
        String ev;
        int indexInMacro;
        public EVEntry(String m, String e, int idx) { macroName = m; ev = e; indexInMacro = idx; }
        public String toString() { return macroName + " : " + ev + " (E#"+indexInMacro+")"; }
    }

    // ---- Main ----
    public static void main(String[] args) throws Exception {
        String inputFile = args.length > 0 ? args[0] : "macro_input.txt";

        List<MNTEntry> MNT = new ArrayList<>();
        List<String> MDT = new ArrayList<>();
        List<KPDTEntry> KPDTAB = new ArrayList<>();
        List<PNTEntry> PNTAB = new ArrayList<>();
        List<EVEntry> EVNTAB = new ArrayList<>();
        List<String> SSNTAB = new ArrayList<>(); // sequencing symbols (minimal use here)
        List<Integer> SSTAB = new ArrayList<>(); // pointers into MDT (minimal)

        int MDT_ptr = 1; // MDT indexing from 1
        // KPDT and PNTAB sizes can be fetched from lists when needed

        BufferedReader br = new BufferedReader(new FileReader(inputFile));
        String rawLine;
        boolean insideMacro = false;
        MNTEntry current = null;

        Pattern ampPattern = Pattern.compile("&([A-Za-z0-9_]+)");

        while ((rawLine = br.readLine()) != null) {
            String line = rawLine.trim();
            if (line.isEmpty()) continue;

            if (!insideMacro) {
                // detect MACRO start (support both "MACRO" and "NAME MACRO ..." forms)
                if (containsWord(line, "MACRO")) {
                    // Determine header line
                    String headerLine;
                    if (line.equalsIgnoreCase("MACRO")) {
                        headerLine = br.readLine();
                        if (headerLine == null) break;
                        headerLine = headerLine.trim();
                    } else {
                        // e.g. "INCR MACRO &A,&B=5"  -> remove the MACRO token
                        headerLine = line.replaceAll("(?i)\\bMACRO\\b", "").trim();
                        if (headerLine.isEmpty()) {
                            headerLine = br.readLine();
                            if (headerLine == null) break;
                            headerLine = headerLine.trim();
                        }
                    }

                    // parse header: macroName [params...]
                    String[] hp = headerLine.split("\\s+", 2);
                    String macroName = hp[0].trim();
                    String paramsStr = hp.length > 1 ? hp[1].trim() : "";

                    MNTEntry me = new MNTEntry();
                    me.name = macroName;
                    me.mdtIndex = MDT_ptr;
                    me.kpdtIndex = KPDTAB.size() + 1; // record start; will set to 0 if no KP
                    me.pntabIndex = PNTAB.size() + 1;
                    me.ppCount = 0;
                    me.kpCount = 0;
                    me.sstp = SSTAB.size() + 1;

                    // parse parameter list (remove surrounding parentheses if any)
                    if (!paramsStr.isEmpty()) {
                        paramsStr = paramsStr.replaceAll("[()]", "");
                        String[] params = paramsStr.split("\\s*,\\s*");
                        for (String p : params) {
                            if (p == null) continue;
                            p = p.trim();
                            if (p.startsWith("&")) p = p.substring(1);
                            if (p.isEmpty()) continue;
                            if (p.contains("=")) {
                                String[] kv = p.split("=", 2);
                                String pname = kv[0].trim();
                                String dval = kv.length > 1 ? kv[1].trim() : "";
                                KPDTAB.add(new KPDTEntry(pname, dval));
                                PNTAB.add(new PNTEntry(macroName, pname));
                                me.paramList.add(pname);
                                me.kpCount++;
                                me.ppCount++;
                            } else {
                                PNTAB.add(new PNTEntry(macroName, p));
                                me.paramList.add(p);
                                me.ppCount++;
                            }
                        }
                    }

                    if (me.kpCount == 0) me.kpdtIndex = 0; // per algorithm

                    MNT.add(me);
                    current = me;
                    insideMacro = true;
                    continue;
                } else {
                    // not a macro start; ignore global source lines for pass1
                    continue;
                }
            } else {
                // inside a macro body until MEND
                if (line.equalsIgnoreCase("MEND") || line.toUpperCase().startsWith("MEND")) {
                    MDT.add("MEND");
                    MDT_ptr++;
                    insideMacro = false;
                    // If no SSNTAB entries for this macro, set SSTP to 0 (as algorithm)
                    if (SSNTAB.isEmpty()) current.sstp = 0;
                    current = null;
                    continue;
                }

                // check LCL (expansion variable) line
                String firstTok = line.split("\\s+", 2)[0];
                if (firstTok.equalsIgnoreCase("LCL")) {
                    String rest = "";
                    String[] parts = line.split("\\s+", 2);
                    if (parts.length > 1) rest = parts[1].trim();
                    // parse expansion variables separated by commas
                    rest = rest.replaceAll("[()]", "");
                    String[] evs = rest.split("\\s*,\\s*");
                    for (String ev : evs) {
                        if (ev == null || ev.isEmpty()) continue;
                        if (ev.startsWith("&")) ev = ev.substring(1);
                        EVNTAB.add(new EVEntry(current.name, ev, current.evList.size() + 1));
                        current.evList.add(ev);
                    }
                    // record LCL line in MDT as-is (we could also replace &ev, but not necessary)
                    MDT.add(line);
                    MDT_ptr++;
                    continue;
                }

                // Preprocessor statements (SET, AIF, AGO) or general model statements:
                // Replace &params with (P,#) and &ev with (E,#)
                String processed = replaceParamsAndEV(line, current, ampPattern);
                // If sequencing symbols appear in label we could add to SSNTAB -- minimal support:
                // (we won't try to detect complex sequencing syntaxes here; user can extend)
                MDT.add(processed);
                MDT_ptr++;
            }
        } // end read loop

        br.close();

        // WRITE OUTPUT FILES
        writeListFile("MNT.txt", prettyMNT(MNT));
        writeListFile("MDT.txt", numberedList(MDT));
        writeListFile("PNTAB.txt", prettyPNT(PNTAB));
        writeListFile("KPDTAB.txt", prettyKPDT(KPDTAB));
        writeListFile("EVNTAB.txt", prettyEV(EVNTAB));
        writeListFile("SSNTAB.txt", prettySSN(SSNTAB));
        writeListFile("SSTAB.txt", prettySST(SSTAB));

        System.out.println("Pass1 complete. Files written: MNT.txt, MDT.txt, PNTAB.txt, KPDTAB.txt, EVNTAB.txt, SSNTAB.txt, SSTAB.txt");
    }

    // ---- Helpers ----

    // Replace &name by (P,#) or (E,#) using current macro's paramList and evList
    static String replaceParamsAndEV(String line, MNTEntry current, Pattern ampPattern) {
        Matcher m = ampPattern.matcher(line);
        StringBuffer sb = new StringBuffer();
        while (m.find()) {
            String name = m.group(1);
            int pIndex = current.paramList.indexOf(name);
            if (pIndex >= 0) {
                String repl = "(P," + (pIndex + 1) + ")";
                m.appendReplacement(sb, Matcher.quoteReplacement(repl));
            } else {
                int eIndex = current.evList.indexOf(name);
                if (eIndex >= 0) {
                    String repl = "(E," + (eIndex + 1) + ")";
                    m.appendReplacement(sb, Matcher.quoteReplacement(repl));
                } else {
                    // not found: leave as &name (or remove & if you'd rather)
                    m.appendReplacement(sb, Matcher.quoteReplacement("&" + name));
                }
            }
        }
        m.appendTail(sb);
        return sb.toString();
    }

    static boolean containsWord(String text, String word) {
        return Pattern.compile("\\b" + Pattern.quote(word) + "\\b", Pattern.CASE_INSENSITIVE).matcher(text).find();
    }

    static void writeListFile(String fname, List<String> lines) throws IOException {
        try (PrintWriter pw = new PrintWriter(new FileWriter(fname))) {
            for (String l : lines) pw.println(l);
        }
    }

    static List<String> numberedList(List<String> lines) {
        List<String> out = new ArrayList<>();
        for (int i = 0; i < lines.size(); i++) {
            out.add(String.format("%3d: %s", i + 1, lines.get(i)));
        }
        return out;
    }

    static List<String> prettyMNT(List<MNTEntry> list) {
        List<String> out = new ArrayList<>();
        out.add("Index  Name   MDT  KPDTP  PNTAB  #PP  #KP  SSTP");
        for (int i = 0; i < list.size(); i++) {
            MNTEntry e = list.get(i);
            out.add(String.format("%3d  %s", i + 1, e.toString()));
            // also show param list for clarity:
            if (!e.paramList.isEmpty()) out.add("     params: " + e.paramList);
            if (!e.evList.isEmpty()) out.add("     EVs: " + e.evList);
        }
        return out;
    }

    static List<String> prettyPNT(List<PNTEntry> list) {
        List<String> out = new ArrayList<>();
        out.add("Index  Macro : Param");
        for (int i = 0; i < list.size(); i++) {
            PNTEntry p = list.get(i);
            out.add(String.format("%3d   %s", i + 1, p.toString()));
        }
        return out;
    }

    static List<String> prettyKPDT(List<KPDTEntry> list) {
        List<String> out = new ArrayList<>();
        out.add("Index  Param = Default");
        for (int i = 0; i < list.size(); i++) {
            out.add(String.format("%3d   %s", i + 1, list.get(i).toString()));
        }
        return out;
    }

    static List<String> prettyEV(List<EVEntry> list) {
        List<String> out = new ArrayList<>();
        out.add("Index  Macro : EV (E#)");
        for (int i = 0; i < list.size(); i++) {
            out.add(String.format("%3d   %s", i + 1, list.get(i).toString()));
        }
        return out;
    }

    static List<String> prettySSN(List<String> list) {
        List<String> out = new ArrayList<>();
        out.add("SSNTAB entries:");
        for (int i = 0; i < list.size(); i++) out.add(String.format("%3d  %s", i + 1, list.get(i)));
        return out;
    }

    static List<String> prettySST(List<Integer> list) {
        List<String> out = new ArrayList<>();
        out.add("SSTAB pointers (index -> MDT index):");
        for (int i = 0; i < list.size(); i++) out.add(String.format("%3d  -> %d", i + 1, list.get(i)));
        return out;
    }
}
