
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PassOne {

    static class OpEntry {

        String mnemonic;
        String code;
        String type;

        public OpEntry(String mnemonic, String code, String type) {
            this.code = code;
            this.mnemonic = mnemonic;
            this.type = type;
        }
    }

    static class Symbol {

        String name;
        Integer address;

        public Symbol(String name, Integer address) {
            this.address = address;
            this.name = name;
        }

    }

    static class Literal {

        String literal;
        Integer address;

        public Literal(String literal) {
            this.literal = literal;
            this.address = null;
        }
    }

    Map<String, OpEntry> OPTAB = new LinkedHashMap<>();
    Map<String, Symbol> SYMTAB = new LinkedHashMap<>();
    Map<Integer, Literal> LITTAB = new LinkedHashMap<>();
    Map<String, Integer> REGTAB = new LinkedHashMap<>();
    List<Integer> POOLTAB = new ArrayList<>();

    List<String> intermediateLines = new ArrayList<>();
    List<String> IC = new ArrayList<>();

    int LOCCNTR = 0;
    int startAddress = 0;
    int literalIndex = 0;

    public PassOne() {
        initOptab();
    }

    private void initOptab() {
        // Imperative statements (IS)
        OPTAB.put("STOP", new OpEntry("STOP", "00", "IS"));
        OPTAB.put("ADD", new OpEntry("ADD", "01", "IS"));
        OPTAB.put("SUB", new OpEntry("SUB", "02", "IS"));
        OPTAB.put("MUL", new OpEntry("MUL", "03", "IS"));
        OPTAB.put("MULT", new OpEntry("MUL", "03", "IS"));
        OPTAB.put("MOVER", new OpEntry("MOVER", "04", "IS"));
        OPTAB.put("MOVEM", new OpEntry("MOVEM", "05", "IS"));
        OPTAB.put("COMP", new OpEntry("COMP", "06", "IS"));
        OPTAB.put("BC", new OpEntry("BC", "07", "IS"));
        OPTAB.put("DIV", new OpEntry("DIV", "08", "IS"));
        OPTAB.put("READ", new OpEntry("READ", "09", "IS"));
        OPTAB.put("PRINT", new OpEntry("PRINT", "10", "IS"));
        OPTAB.put("JUMP", new OpEntry("JUMP", "11", "IS"));

        // Declarative (DL)
        OPTAB.put("DC", new OpEntry("DC", "01", "DL"));
        OPTAB.put("DS", new OpEntry("DS", "02", "DL"));

        // Assembler Directives (AD)
        OPTAB.put("START", new OpEntry("START", "01", "AD"));
        OPTAB.put("END", new OpEntry("END", "02", "AD"));
        OPTAB.put("ORIGIN", new OpEntry("ORIGIN", "03", "AD"));
        OPTAB.put("EQU", new OpEntry("EQU", "04", "AD"));
        OPTAB.put("LTORG", new OpEntry("LTORG", "05", "AD"));

        REGTAB.put("AREG", 1);
        REGTAB.put("BREG", 2);
        REGTAB.put("CREG", 3);
        REGTAB.put("DREG", 4);
    }
    
    public void processFile(String inoutPath) throws IOException {
        List<String> lines = Files.readAllLines(Paths.get(inoutPath));

        POOLTAB.add(0);   // as index from 1 

        for (String raw : lines) {
            String line = raw.trim();
            if (line.isEmpty() || line.startsWith(";")) {
                continue;  // empty line or comment
            }

            String label = null;
            String opcode;
            String operands = null;
            String[] parts = splitLine(line);

            if (parts.length == 3) {
                label = parts[0];
                opcode = parts[1];
                operands = parts[2];
            } else if (parts.length == 2) {
                opcode = parts[0];
                operands = parts[1];
            } else {
                opcode = parts[0];
            }

            opcode = opcode.toUpperCase();

            if (label != null) {
                if (SYMTAB.containsKey(label)) {
                    Symbol s = SYMTAB.get(label);
                    if (s.address == null) {
                        s.address = LOCCNTR;
                    }
                } else {
                    SYMTAB.put(label, new Symbol(label, LOCCNTR));
                }
            }

            OpEntry op = OPTAB.get(opcode);
            if (op == null) {
                op = new OpEntry(opcode, "??", "IS");
                OPTAB.put(opcode, op);
            }

            switch (op.type) {
                case "AD":
                    handleAssemblerDirective(opcode, operands, label);
                    break;

                case "IS":
                    if (operands != null) collectLiterals(operands);
                    IC.add(createIC(LOCCNTR, label, opcode, operands));
                    LOCCNTR++;
                    break;

                case "DL":
                    if (opcode.equals("DC")) {
                        IC.add(createIC(LOCCNTR, label, opcode, operands));
                        LOCCNTR++;
                    } else if (opcode.equals("DS")) {
                        int size = 1;
                        if (operands != null) {
                            try { size = Integer.parseInt(operands.trim()); }
                            catch (Exception e) { size = 1; }
                        }
                        IC.add(createIC(LOCCNTR, label, opcode, operands));
                        LOCCNTR += size;
                    }
                    break;

                default:
                    IC.add(createIC(LOCCNTR, label, opcode, operands));
                    LOCCNTR++;
                    break;
            }
        }

        processLiteralPool(true);

        // --- Write outputs for Pass2 ---
        Files.write(Paths.get("IC.txt"), IC);   // instead of intermediate.txt

        List<String> symLines = new ArrayList<>();
        symLines.add("Symbol\tAddress");
        for (Symbol s : SYMTAB.values()) {
            symLines.add(s.name + "\t" + (s.address == null ? "0" : s.address));
        }
        Files.write(Paths.get("SYM.txt"), symLines);

        List<String> litLines = new ArrayList<>();
        litLines.add("Literal\tAddress");
        for (Literal l : LITTAB.values()) {
            litLines.add(l.literal + "\t" + (l.address == null ? "0" : l.address));
        }
        Files.write(Paths.get("LIT.txt"), litLines);

        // pooltab only for debugging
        List<String> poolLines = new ArrayList<>();
        for (int i = 0; i < POOLTAB.size(); i++) {
            poolLines.add("Pool " + (i + 1) + " starts at literal index " + (POOLTAB.get(i) + 1));
        }
        Files.write(Paths.get("POOLTAB.txt"), poolLines);

        System.out.println("\nPass-I completed. Generated IC.txt, SYM.txt, LIT.txt");
    }




    private void collectLiterals(String operands) {
        if (operands == null) {
            return;
        }
        // find all occurrences starting with =
        Matcher m = Pattern.compile("=(C'[^']*'|X'[^']*'|'[^']*'|\\d+|[^,\\s]+)").matcher(operands);
        while (m.find()) {
            String lit = m.group();
            if (!LITTAB.containsKey(lit)) {
                LITTAB.put(literalIndex++, new Literal(lit));
            }
            System.out.println("Literal: " + lit);
        }
    }

    private void handleAssemblerDirective(String opcode, String operands, String label) {
        switch (opcode) {
            case "START":
                if (operands != null) {
                    try {
                        startAddress = Integer.parseInt(operands.trim());
                        LOCCNTR = startAddress;
                    } catch (NumberFormatException e) {
                        LOCCNTR = 0;
                    }
                }
                break;
            case "END":
                processLiteralPool(true);
                break;

            case "LTORG":
                processLiteralPool(false);
                break;
            case "ORIGIN":
                if (operands != null) {
                    Integer val = evaluateExpression(operands.trim());
                    if (val != null) {
                        LOCCNTR = val;
                    }
                }
                break;

            case "EQU":
                // operands expected as value or expression. Label must have been added already in SYMTAB
                // Note: This handler will be called during general processing where label has been placed
                // So set its value from operand expression
                if (operands != null) {
                    Integer val = evaluateExpression(operands.trim());
                    Symbol l = SYMTAB.get(label);
                    l.address = val;
                }
            default:
                break;
        }
    }

    private void processLiteralPool(boolean finalPool) {
        int poolStartIndex = POOLTAB.get(POOLTAB.size() - 1);
        List<Integer> keys = new ArrayList<>(LITTAB.keySet());
        boolean anyAssigned = false;
        for (int i = poolStartIndex; i < keys.size(); i++) {
            Literal lit = LITTAB.get(i);
            if (lit.address == null) {
                lit.address = LOCCNTR;
                LOCCNTR++;
                anyAssigned = true;
                intermediateLines.add(formatIntermediate(lit.address, null, "(LITERAL)", lit.literal));
                // intermediateLines.add(createIC(lit.address, null, "(LITERAL)", lit.literal));
            }
            System.out.println("Assigning address to literal: " + lit.literal + " at " + lit.address);
        }
        if (anyAssigned) {
            // start new pool at current LITTAB size
            POOLTAB.add(LITTAB.size());
        }

    }

    private String[] splitLine(String line) {
        // Attempt to split into label/opcode/operands by spaces or tabs, but operands may contain commas.
        // We'll use a regex: optional label (ends with ':' or first token followed by whitespace and an opcode), but keep it simple.
        String[] tokens = line.split("\t+| +");
        if (tokens.length >= 3) {
            // label opcode operands...
            if (OPTAB.containsKey(tokens[0])) {
                String opcode = tokens[0];
                String operands = String.join(" ", Arrays.copyOfRange(tokens, 1, tokens.length)).trim();
                return new String[]{opcode, operands};
            } else {
                String label = tokens[0];
                String opcode = tokens[1];
                String operands = String.join(" ", Arrays.copyOfRange(tokens, 2, tokens.length)).trim();
                return new String[]{label, opcode, operands};
            }
        } else if (tokens.length == 2) {
            if (OPTAB.containsKey(tokens[1])) {
                return new String[]{tokens[0], tokens[1], null};
            } else {
                return new String[]{tokens[0], tokens[1]};
            }

        } else {
            return new String[]{tokens[0]};
        }
    }

    private void printTables() {
        System.out.println("\n--- Intermediate file written :\n");
        for (int i = 0; i < Math.min(intermediateLines.size(), 30); i++) {
            System.out.println(intermediateLines.get(i));
        }

        System.out.println("\nSYMTAB");
        System.out.printf("Symbol \t Address\n");
        for (Symbol s : SYMTAB.values()) {
            System.out.printf("%-10s %-6s\n", s.name, (s.address == null ? "--" : s.address));
        }

        System.out.println("\n--- LITTAB ---");
        System.out.printf("%-15s %-8s\n", "Literal", "Address");
        for (Literal l : LITTAB.values()) {
            System.out.printf("%-15s %-8s\n", l.literal, (l.address == null ? "--" : l.address));
        }

        System.out.println("\n--- POOLTAB ---");
        for (int i = 0; i < POOLTAB.size(); i++) {
            System.out.printf("Pool %d starts at literal index %d\n", i + 1, POOLTAB.get(i) + 1);
        }

        System.out.println("\nLocation counter end: " + LOCCNTR + "   Start address: " + startAddress);

    }

    private String formatIntermediate(Integer loc, String label, String opcode, String operands) {
        String locStr = (loc == null) ? "    " : String.format("%04d", loc);
        return String.join("\t", Arrays.asList(locStr, (label == null ? "" : label), (opcode == null ? "" : opcode), (operands == null ? "" : operands)));
    }

    private String createIC(Integer loc, String label, String opcode, String operands) {
    StringBuilder sb = new StringBuilder();

    // Location Counter
    String locStr = (loc == null) ? "    " : String.format("%04d", loc);
    sb.append(locStr).append("\t");

    OpEntry op = OPTAB.get(opcode);
    if (op == null) {
        sb.append("(??,??)\t");
        return sb.toString();
    }

    sb.append("(").append(op.type).append(",").append(op.code).append(")\t");

    if (operands != null && !operands.isEmpty()) {
        String[] ops = operands.split("[, ]+");
        for (String o : ops) {
            o = o.trim();
            if (o.isEmpty()) continue;

            // Register
            if (REGTAB.containsKey(o)) {
                sb.append("(R,").append(REGTAB.get(o)).append(")\t");
            }
            // Literal
            else if (o.startsWith("=")) {
                for (Map.Entry<Integer, Literal> e : LITTAB.entrySet()) {
                    if (e.getValue().literal.equals(o)) {
                        sb.append("(L,").append(e.getKey() + 1).append(")\t");
                        break;
                    }
                }
            }
            // Symbol
            else if (SYMTAB.containsKey(o)) {
                int symIndex = new ArrayList<>(SYMTAB.keySet()).indexOf(o) + 1;
                sb.append("(S,").append(symIndex).append(")\t");
            }
            // Constant (number)
            else {
                try {
                    int val = Integer.parseInt(o);
                    sb.append("(C,").append(val).append(")\t");
                } catch (Exception e) {
                    // forward reference → add symbol if not exists
                    if (!SYMTAB.containsKey(o)) {
                        SYMTAB.put(o, new Symbol(o, null));
                    }
                    int symIndex = new ArrayList<>(SYMTAB.keySet()).indexOf(o) + 1;
                    sb.append("(S,").append(symIndex).append(")\t");
                }
            }
        }
    }

    return sb.toString().trim();
}


    private Integer evaluateExpression(String expr) {
        expr = expr.replaceAll("\\s+", "");
        try {
            return Integer.parseInt(expr);
        } catch (Exception ignored) {
        }

        // symbol +/- number
        Matcher m = Pattern.compile("([A-Za-z_][A-Za-z0-9_]*)?([+-]?)(\\d+)?").matcher(expr);
        if (m.matches()) {
            String sym = m.group(1);
            String sign = m.group(2);
            String num = m.group(3);
            int n = 0;
            if (num != null && !num.isEmpty()) {
                n = Integer.parseInt(num);
            }
            if (sym != null && !sym.isEmpty()) {
                Symbol s = SYMTAB.get(sym);
                Integer addr = (s != null) ? s.address : null;
                if (addr == null) {
                    System.err.println("Error: undefined symbol in expression: " + sym);
                    return null;
                }
                if (sign.equals("-")) {
                    return addr - n;
                } else {
                    return addr + n; // sign could be + or empty

                }
            } else {
                // no symbol but maybe +/- number
                if (sign.equals("-")) {
                    return -n;
                } else {
                    return n;
                }
            }
        }
        System.err.println("Could not evaluate expression: " + expr);
        return null;
    }

    public static void main(String[] args) throws Exception {
        // For demonstration include a sample assembly into "input.asm" if it doesn't exist
        Path in = Paths.get("input.asm");
        if (!Files.exists(in)) {
            List<String> sample = Arrays.asList(
                    "START 100",
                    "FIRST  MOV A, ='=5'", // example with a literal (toy)
                    "       ADD B, =C'\'X\''",
                    "LABEL1 SUB A, B",
                    "       LTORG",
                    "LOOP   DS 2",
                    "CONST  DC 5",
                    "VALUE  EQU CONST",
                    "       ORIGIN 110",
                    "NEW    MOV A, VALUE",
                    "       END"
            );
            Files.write(in, sample);
            System.out.println("Sample input.asm created (you can edit it and run again).\n");
        }

        PassOne pass1 = new PassOne();
        pass1.processFile("input.asm");
        // pass1.processFile("input2.asm");

        System.out.println("\nPass-I completed. Check intermediate.txt and printed tables above.");
    }
}
