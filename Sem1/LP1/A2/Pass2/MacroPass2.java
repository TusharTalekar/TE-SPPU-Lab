import java.io.*;
import java.util.*;
import java.util.regex.*;

public class MacroPass2 {

    static class MNTEntry {
        String name;
        int mdtIndex,kpdtIndex,pntabIndex,ppCount,kpCount,sstp;
        MNTEntry(String n,int m,int k,int p,int pp,int kp,int s) { name=n; mdtIndex=m; kpdtIndex=k; pntabIndex=p; ppCount=pp; kpCount=kp; sstp=s; }
    }

    static class KPDTEntry {
        String name,val; KPDTEntry(String n,String v){name=n;val=v;}
    }

    public static void main(String[] args) throws Exception {

        // Load Pass1 raw tables
        List<MNTEntry> MNT = new ArrayList<>();
        List<KPDTEntry> KPDTAB = new ArrayList<>();
        List<String> MDT = new ArrayList<>();

        BufferedReader br = new BufferedReader(new FileReader("MNT_raw.txt"));
        String line;
        while((line=br.readLine())!=null) {
            String[] p = line.split("\\s+");
            MNT.add(new MNTEntry(p[0],Integer.parseInt(p[1]),Integer.parseInt(p[2]),
                    Integer.parseInt(p[3]),Integer.parseInt(p[4]),Integer.parseInt(p[5]),Integer.parseInt(p[6])));
        }
        br.close();

        br = new BufferedReader(new FileReader("KPDTAB_raw.txt"));
        while((line=br.readLine())!=null) {
            String[] p = line.split("\\s+",2);
            KPDTAB.add(new KPDTEntry(p[0],p.length>1?p[1]:""));
        }
        br.close();

        br = new BufferedReader(new FileReader("MDT_raw.txt"));
        while((line=br.readLine())!=null) MDT.add(line);
        br.close();

        // Read macro calls
        br = new BufferedReader(new FileReader("macro_calls.txt"));
        List<String> calls = new ArrayList<>();
        while((line=br.readLine())!=null) if(!line.trim().isEmpty()) calls.add(line.trim());
        br.close();

        PrintWriter out = new PrintWriter("expanded_output.txt");

        for(String call: calls) {
            String[] parts = call.split("\\s+",2);
            String macroName = parts[0];
            String paramStr = parts.length>1?parts[1]:"";

            MNTEntry mnt=null;
            for(MNTEntry e:MNT) if(e.name.equals(macroName)) {mnt=e;break;}
            if(mnt==null) continue;

            String[] actuals = paramStr.replaceAll("[()]","").split("\\s*,\\s*");
            String[] APTAB = new String[mnt.ppCount+mnt.kpCount+1];

            // Copy default KP values
            for(int i=0;i<mnt.kpCount;i++) {
                KPDTEntry kp = KPDTAB.get(mnt.kpdtIndex-1+i);
                APTAB[mnt.ppCount-i-1] = kp.val; // simple mapping
            }

            // Override with actuals
            for(int i=0;i<actuals.length;i++) APTAB[i]=actuals[i];

            // Expand MDT
            for(int idx=mnt.mdtIndex-1; idx<MDT.size(); idx++) {
                String stmt=MDT.get(idx);
                if(stmt.equalsIgnoreCase("MEND")) break;

                Pattern p = Pattern.compile("\\(P,(\\d+)\\)");
                Matcher m = p.matcher(stmt);
                StringBuffer sb = new StringBuffer();
                while(m.find()){
                    int pi=Integer.parseInt(m.group(1));
                    m.appendReplacement(sb, APTAB[pi-1]!=null?APTAB[pi-1]:"");
                }
                m.appendTail(sb);
                stmt=sb.toString();

                Pattern e = Pattern.compile("\\(E,(\\d+)\\)");
                Matcher me = e.matcher(stmt);
                sb=new StringBuffer();
                while(me.find()){
                    // expansion vars: can be left empty for simplicity
                    me.appendReplacement(sb,"");
                }
                me.appendTail(sb);
                stmt=sb.toString();

                out.println(stmt);
            }
        }

        out.close();
        System.out.println("Macro expansion done. Output: expanded_output.txt");
    }
}
