import java.io.*;
import java.util.*;
import java.util.regex.*;

public class Pass1 {

    // Table classes
    static class MNTEntry {
        String name;
        int mdtIndex, kpdtIndex, pntabIndex, ppCount, kpCount, sstp;
        List<String> paramList = new ArrayList<>();
        List<String> evList = new ArrayList<>();
    }

    static class KPDTEntry {
        String name, defaultVal;
        KPDTEntry(String n, String d) { name = n; defaultVal = d; }
    }

    static class PNTEntry {
        String macro, param;
        PNTEntry(String m, String p) { macro = m; param = p; }
    }

    static class EVEntry {
        String macro, ev;
        int idx;
        EVEntry(String m, String e, int i) { macro = m; ev = e; idx = i; }
    }

    public static void main(String[] args) throws Exception {
        String inputFile = args.length > 0 ? args[0] : "macro_input.txt";

        List<MNTEntry> MNT = new ArrayList<>();
        List<String> MDT = new ArrayList<>();
        List<KPDTEntry> KPDTAB = new ArrayList<>();
        List<PNTEntry> PNTAB = new ArrayList<>();
        List<EVEntry> EVTAB = new ArrayList<>();

        int MDT_ptr = 1;

        BufferedReader br = new BufferedReader(new FileReader(inputFile));
        String line;
        boolean insideMacro = false;
        MNTEntry current = null;

        Pattern amp = Pattern.compile("&([A-Za-z0-9_]+)");

        while ((line = br.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty()) continue;

            if (!insideMacro) {
                if (line.equalsIgnoreCase("MACRO")) {
                    String header = br.readLine().trim();
                    String[] parts = header.split("\\s+",2);
                    String macroName = parts[0];
                    String paramStr = parts.length>1?parts[1]:"";

                    current = new MNTEntry();
                    current.name = macroName;
                    current.mdtIndex = MDT_ptr;
                    current.kpdtIndex = KPDTAB.size()+1;
                    current.pntabIndex = PNTAB.size()+1;
                    current.ppCount = 0;
                    current.kpCount = 0;

                    // Parse parameters
                    if(!paramStr.isEmpty()) {
                        paramStr = paramStr.replaceAll("[()]","");
                        String[] params = paramStr.split("\\s*,\\s*");
                        for(String p: params) {
                            p = p.replace("&","");
                            if(p.contains("=")) {
                                String[] kv = p.split("=",2);
                                KPDTAB.add(new KPDTEntry(kv[0], kv[1]));
                                PNTAB.add(new PNTEntry(macroName, kv[0]));
                                current.paramList.add(kv[0]);
                                current.ppCount++;
                                current.kpCount++;
                            } else {
                                PNTAB.add(new PNTEntry(macroName, p));
                                current.paramList.add(p);
                                current.ppCount++;
                            }
                        }
                    }
                    if(current.kpCount==0) current.kpdtIndex=0;
                    insideMacro = true;
                    MNT.add(current);
                }
            } else {
                if(line.equalsIgnoreCase("MEND")) {
                    MDT.add("MEND");
                    MDT_ptr++;
                    insideMacro = false;
                    current = null;
                    continue;
                }

                // LCL expansion vars
                if(line.toUpperCase().startsWith("LCL")) {
                    String rest = line.split("\\s+",2)[1].replaceAll("[()]","");
                    String[] evs = rest.split("\\s*,\\s*");
                    for(String ev: evs) {
                        ev = ev.replace("&","");
                        EVTAB.add(new EVEntry(current.name, ev, current.evList.size()+1));
                        current.evList.add(ev);
                    }
                    MDT.add(line);
                    MDT_ptr++;
                    continue;
                }

                // Replace params &ev with (P,#) or (E,#)
                Matcher m = amp.matcher(line);
                StringBuffer sb = new StringBuffer();
                while(m.find()) {
                    String n = m.group(1);
                    int pidx = current.paramList.indexOf(n);
                    if(pidx>=0) m.appendReplacement(sb,"(P,"+(pidx+1)+")");
                    else {
                        int eidx = current.evList.indexOf(n);
                        if(eidx>=0) m.appendReplacement(sb,"(E,"+(eidx+1)+")");
                        else m.appendReplacement(sb,"&"+n);
                    }
                }
                m.appendTail(sb);
                MDT.add(sb.toString());
                MDT_ptr++;
            }
        }
        br.close();

        // Write raw files for Pass2
        writeMNTRaw("MNT_raw.txt", MNT);
        writeKPDTABRaw("KPDTAB_raw.txt", KPDTAB);
        writeMDTRaw("MDT_raw.txt", MDT);
        System.out.println("Pass1 complete. Raw tables written for Pass2.");
    }

    static void writeMNTRaw(String fname, List<MNTEntry> list) throws Exception {
        PrintWriter pw = new PrintWriter(fname);
        for(MNTEntry e: list) {
            pw.println(e.name+" "+e.mdtIndex+" "+e.kpdtIndex+" "+e.pntabIndex+" "+e.ppCount+" "+e.kpCount+" "+e.sstp);
        }
        pw.close();
    }

    static void writeKPDTABRaw(String fname, List<KPDTEntry> list) throws Exception {
        PrintWriter pw = new PrintWriter(fname);
        for(KPDTEntry e: list) pw.println(e.name+" "+e.defaultVal);
        pw.close();
    }

    static void writeMDTRaw(String fname, List<String> list) throws Exception {
        PrintWriter pw = new PrintWriter(fname);
        for(String s: list) pw.println(s);
        pw.close();
    }
}
