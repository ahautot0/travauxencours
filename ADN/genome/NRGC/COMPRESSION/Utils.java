import java.io.*;
import java.util.*;
import java.util.Date;
import java.lang.management.*;

public class Utils {

    public static HashMap<Integer, List<NewKMer>> hashMap;
    public static String reference;
    public static String target;
    public static int kMerLength = 21;
    public static String text = "";
    public static int fixedSize = 20000;
    public static int bit, eit, bir, eir;
    public static boolean DEBUG = false;
    public static String meta_data = "";
    public static int length = 0;
    public static int threshold = 80;


    public Utils() throws IOException {
        text = "";
        hashMap = new HashMap<Integer, List<NewKMer>>();
    }

    public static long getCpuTime() {
        ThreadMXBean bean = ManagementFactory.getThreadMXBean();
        return bean.isCurrentThreadCpuTimeSupported() ? bean.getCurrentThreadCpuTime() : 0L;
    }

    public static String readSequence(String sequenceFileName) throws IOException {
        FileInputStream fileInputStream = new FileInputStream(sequenceFileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        boolean store = false;
        StringBuilder strBuf = new StringBuilder();
        while ((line = bufferedReader.readLine()) != null) {
            if (!line.startsWith(">") && !line.startsWith("@")) {
                strBuf.append(line);
                if (!store) {
                    length = line.length();
                    store = true;
                }
            } else {
                meta_data = line;
            }
        }
        return strBuf.toString();
    }

    public static void buildKMerFromRead(String read, int kMerLength) {
        int iteration = read.length() - kMerLength + 1;

        for (int i = 0; i < iteration; i++) {
            String kMer = read.substring(i, i + kMerLength);

            NewKMer newKMer = new NewKMer();
            newKMer.setStartPosition(i);
            newKMer.setkMer(kMer);
            int key = kMer.hashCode();

            if (hashMap.containsKey(key)) {
                List<NewKMer> list = hashMap.get(key);
                list.add(newKMer);
            } else {
                List<NewKMer> list = new ArrayList<NewKMer>();
                list.add(newKMer);
                hashMap.put(key, list);
            }
        }
    }

    public static int getPosition(String kMer, int exclude) {
        int position = Integer.MAX_VALUE;
        int key = kMer.hashCode();

        if (!hashMap.containsKey(key)) {
            return position;
        }

        List<NewKMer> list = hashMap.get(key);

        for (NewKMer newKMer : list) {
            if (newKMer.getkMer().equals(kMer)) {
                int startPosition = newKMer.getStartPosition();
                if (startPosition > exclude && startPosition < exclude + threshold) {
                    if (startPosition < position) {
                        position = startPosition;
                    }
                }
            }
        }
        return position;
    }

    public static List<Position> get_ns__position_list(String sequence) {
        List<Position> list = new ArrayList<Position>();
        boolean accept = false;
        int starting_position = 0, ending_position = 0;

        for (int i = 0; i < sequence.length(); i++) {
            if (sequence.charAt(i) == 'n' || sequence.charAt(i) == 'N') {
                if (accept) {
                    ending_position += 1;
                } else {
                    starting_position = i;
                    ending_position += 1;
                    accept = true;
                }
            } else {
                if (accept) {
                    Position position = new Position();
                    position.setBeginIndexTarget(starting_position);
                    position.setEndIndexTarget(ending_position);
                    list.add(position);
                }
                accept = false;
                starting_position = 0;
                ending_position = 0;
            }
        }

        if (accept) {
            Position position = new Position();
            position.setBeginIndexTarget(starting_position);
            position.setEndIndexTarget(ending_position);
            list.add(position);
        }

        return list;
    }

    public static List<Position> get_lower_letters_position_list(String sequence) {
        List<Position> list = new ArrayList<Position>();
        boolean accept = false;
        int starting_position = 0, ending_position = 0, counter = 0;

        for (int i = 0; i < sequence.length(); i++) {
            if (Character.isLowerCase(sequence.charAt(i))) {
                counter++;
                if (accept) {
                    ending_position += 1;
                } else {
                    starting_position = i;
                    ending_position += 1;
                    accept = true;
                }
            } else {
                if (accept) {
                    Position position = new Position();
                    position.setBeginIndexTarget(starting_position);
                    position.setEndIndexTarget(ending_position);
                    list.add(position);
                }
                accept = false;
                starting_position = 0;
                ending_position = 0;
            }
        }

        if (accept) {
            Position position = new Position();
            position.setBeginIndexTarget(starting_position);
            position.setEndIndexTarget(ending_position);
            list.add(position);
        }
        System.out.println(counter);
        return list;
    }

    public static List<Position> get_upper_letters_position_list(String sequence) {
        List<Position> list = new ArrayList<Position>();
        boolean accept = false;
        int starting_position = 0, ending_position = 0, counter = 0;

        for (int i = 0; i < sequence.length(); i++) {
            if (Character.isUpperCase(sequence.charAt(i))) {
                counter++;
                if (accept) {
                    ending_position += 1;
                } else {
                    starting_position = i;
                    ending_position += 1;
                    accept = true;
                }
            } else {
                if (accept) {
                    Position position = new Position();
                    position.setBeginIndexTarget(starting_position);
                    position.setEndIndexTarget(ending_position);
                    list.add(position);
                }
                accept = false;
                starting_position = 0;
                ending_position = 0;
            }
        }

        if (accept) {
            Position position = new Position();
            position.setBeginIndexTarget(starting_position);
            position.setEndIndexTarget(ending_position);
            list.add(position);
        }

        System.out.println(counter);
        return list;
    }

    public static List<Position> get_position_list(String sequence) throws IOException {
        List<Position> list = new ArrayList<Position>();
        boolean accept = false;
        int starting_position = 0, ending_position = 0;

        for (int i = 0; i < sequence.length(); i++) {
            if (sequence.charAt(i) == 'N' || sequence.charAt(i) == 'n') {
                if (accept) {
                    ending_position += 1;
                } else {
                    starting_position = i;
                    ending_position += 1;
                    accept = true;
                }
            } else {
                if (accept) {
                    Position position = new Position();
                    position.setBeginIndexTarget(starting_position);
                    position.setEndIndexTarget(ending_position);
                    list.add(position);
                }
                accept = false;
                starting_position = 0;
                ending_position = 0;
            }
        }

        if (accept) {
            Position position = new Position();
            position.setBeginIndexTarget(starting_position);
            position.setEndIndexTarget(ending_position);
            list.add(position);
        }
        return list;
    }

    public static int getPosition(String kMer) {
        int position = Integer.MAX_VALUE;
        int key = kMer.hashCode();

        if (!hashMap.containsKey(key)) {
            return position;
        }

        List<NewKMer> list = hashMap.get(key);

        for (NewKMer newKMer : list) {
            if (newKMer.getkMer().equals(kMer)) {
                int startPosition = newKMer.getStartPosition();
                if (startPosition < position) {
                    position = startPosition;
                }
            }
        }
        return position;
    }

    public static int getIncrement(int refEndPosition, int tarEndPosition, int refEndIndex, int tarEndIndex) {
        int position = 0;
        int endIndex;

        if (refEndIndex - refEndPosition <= tarEndIndex - tarEndPosition) {
            endIndex = refEndIndex - refEndPosition + 1;
        } else {
            endIndex = tarEndIndex - tarEndPosition + 1;
        }


        for (int i = 1; i < endIndex; i++) {
            if (target.charAt(tarEndPosition + i) == reference.charAt(refEndPosition + i)) {
                position++;
            } else {
                break;
            }
        }
        return position;
    }

    public static List<Position> run(String reference, String target, int kMerLength) {
        List<Position> list = new ArrayList<Position>();
        int index = 0, startIndex, exclude = -1;
        String kMer;

        buildKMerFromRead(reference, kMerLength);

        while (true) {
            kMer = target.substring(index, index + kMerLength);

            if (exclude < 0) {
                startIndex = getPosition(kMer);
            } else {
                startIndex = getPosition(kMer, exclude);
            }

            if (startIndex == Integer.MAX_VALUE) {
                index = index + 1;
                if (index + kMerLength > target.length()) {
                    break;
                }
                continue;
            }

            int refEndPosition = startIndex + kMerLength - 1;
            int tarEndPosition = index + kMerLength - 1;
            int refEndIndex = reference.length() - 1;
            int tarEndIndex = target.length() - 1;

            int increment = getIncrement(refEndPosition, tarEndPosition, refEndIndex, tarEndIndex);

            Position position = new Position();
            position.setBeginIndexTarget(index);
            position.setEndIndexTarget(index + kMerLength + increment - 1);
            position.setBeginIndexRef(startIndex);
            position.setEndIndexRef(startIndex + kMerLength + increment - 1);

            list.add(position);

            index = index + kMerLength + increment + 1;
            exclude = startIndex + kMerLength + increment;

            if (index + kMerLength > target.length()) {
                break;
            }
        }
        return list;
    }

    public static Position processPositionList(List<Position> list) {
        int beginIndexTarget, beginIndexRef, endIndexRef;
        for (int i = 0; i < list.size(); i++) {

            if (i == 0) {
                beginIndexTarget = list.get(i).getBeginIndexTarget();
                beginIndexRef = list.get(i).getBeginIndexRef();
                endIndexRef = list.get(i).getEndIndexRef();

                if (beginIndexTarget > 0) {
                    String preamble = target.substring(0, beginIndexTarget);
                    text += preamble + "\n";
                    if (DEBUG) {
                        System.out.println("SIZE: " + preamble.length());
                    }
                }
                text += "" + (beginIndexRef + bir) + "," + (endIndexRef + bir) + "\n";
                continue;
            }

            beginIndexTarget = list.get(i).getBeginIndexTarget();
            beginIndexRef = list.get(i).getBeginIndexRef();
            endIndexRef = list.get(i).getEndIndexRef();

            int endIndexTargetPrev = list.get(i - 1).getEndIndexTarget();
            String interimTarget = target.substring(endIndexTargetPrev + 1, beginIndexTarget);

            if (interimTarget.length() > 0) {
                text += interimTarget + "\n";
            }

            text += (beginIndexRef + bir) + "," + (endIndexRef + bir) + "\n";
        }
        return list.get(list.size() - 1);
    }

    public static void write(String fileName, String text, boolean append) throws IOException {
        FileWriter fileWriter = new FileWriter(fileName, append);
        BufferedWriter out = new BufferedWriter(fileWriter);
        out.write(text);
        out.close();
    }

    public static void write(String fileName, List<Position> list, boolean append, String prefix, String suffix) throws IOException {
        FileWriter fileWriter = new FileWriter(fileName, append);
        BufferedWriter out = new BufferedWriter(fileWriter);
        int prev = 0;
        String text;
        out.write(prefix);
        for (Position position : list) {
            int start = position.getBeginIndexTarget();
            int end = position.getEndIndexTarget();
            int current = start - prev;
            text = current + " " + end + "\n";
            prev = start;
            out.write(text);
        }
        out.write(suffix);
        out.close();
    }

    public static int calculateHD(String readOne, String readTwo) {
        int hammingDistance = 0;

        for (int i = 0; i < readOne.length(); i++) {
            if (readOne.charAt(i) != readTwo.charAt(i)) {
                hammingDistance++;
            }
        }
        return hammingDistance;
    }

    public static int calculateHD(String readOne, String readTwo, int length) {
        int hammingDistance = 0;

        for (int i = 0; i < length; i++) {
            if (readOne.charAt(i) != readTwo.charAt(i)) {
                hammingDistance++;
            }
        }
        return hammingDistance;
    }

    public static boolean is_upper_case(String sequence) throws IOException {

        for (char c : sequence.toCharArray()) {
            if (!Character.isUpperCase(c))
                return false;
        }
        return true;
    }

    public static boolean is_lower_case(String sequence) throws IOException {

        for (char c : sequence.toCharArray()) {
            if (!Character.isLowerCase(c))
                return false;
        }
        return true;
    }

    public static boolean run(String _reference, String _target, String final_file, int begin_index, int end_index) throws IOException {

        kMerLength = 21;
        threshold = 200;
        fixedSize = 30000;
        int counter = 0;
        boolean reject = false;

        boolean traverse = false;
        for (int _iteration = 0; _iteration < 2; _iteration++) {

            String outFile = "temp_out.txt";

            String seq_reference = _reference;
            String seq_target = _target;

            if (_iteration == 0) {
                kMerLength = 13;
                threshold = 400; //400
                fixedSize = 20000;
            } else if (_iteration == 1) {
                threshold = 300; //300
                kMerLength = 12;
                fixedSize = 20000;
                traverse = false;
            }

            File file = new File(outFile);

            if (file.exists()) {
                file.delete();
            }

            int iteration = (int) ((double) seq_target.length() / fixedSize);

            bit = 0;
            eit = fixedSize;
            bir = 0;
            eir = fixedSize;
            Position position = new Position();

            boolean accept = false;

            while (true) {

                if (DEBUG) {
                    System.out.println("BIT: " + bit + " EIT: " + eit + " BIR: " + bir + " EIR: " + eir);
                }

                Utils utils = new Utils();
                int kMerLength = Utils.kMerLength;

                if (eir > seq_reference.length()) {
                    text = (seq_target.substring(bit));
                    if (text.length() <= 0) {
                        traverse = false;
                        break;
                    } else {
                        write(outFile, text, true);
                        traverse = false;
                        break;
                    }
                }

                if (eit > seq_target.length()) {
                    text = (seq_target.substring(bit));
                    if (text.length() <= 0) {
                        break;
                    } else {
                        write(outFile, text, true);
                        break;
                    }
                }

                reference = (seq_reference.substring(bir, eir));
                target = (seq_target.substring(bit, eit));

                List<Position> list = run(reference, target, kMerLength);
                int __bir = bir;
                int __eir = eir;

                if (list.size() <= 0) {

                    counter++;
                    if (counter > 5) {
                        reject = true;
                    }

                    if (_iteration == 0) {
                        traverse = true;
                        break;
                    } else {
                        traverse = false;
                    }

                    threshold = 300;
                    kMerLength = 11;


                    list = run(reference, target, kMerLength);
                    accept = true;

                }
                if (DEBUG) {
                    System.out.println("TARGET SIZE: " + target.length() + "  REFERENCE SIZE: " + reference.length() +
                            "  POSITION LIST: " + list.size());
                }

                if (list.size() <= 0) {

                    counter++;
                    if (counter > 5) {
                        reject = true;
                    }


                    text += target + "\n";
                    write(outFile, text, true);

                    bit += fixedSize;
                    int temp_eit = eit;
                    eit = bit + fixedSize;
                    eir += fixedSize;

                    int difference = seq_target.length() - bit;

                    if (difference <= kMerLength) {

                        if (difference <= 0) {
                            text = (seq_target.substring(temp_eit));
                            write(outFile, text, true);
                        } else {

                            text = (seq_target.substring(bit));
                            write(outFile, text, true);
                        }
                        break;
                    } else if (difference < fixedSize) {
                        eit = seq_target.length() - 1;
                    }

                    int difference_ref = seq_reference.length() - bir;

                    if (difference_ref < fixedSize) {
                        eir = seq_reference.length() - 1;
                    }

                    if (eit >= seq_target.length()) {
                        break;
                    }

                    if (difference_ref <= kMerLength) {
                        text = (seq_target.substring(bit));
                        if (text.length() <= 0) {
                            break;
                        } else {
                            write(outFile, text, true);
                            break;
                        }
                    }
                    continue;
                }

                int _bit = bit;
                int _eit = eit;
                int _bir = bir;
                int _eir = eir;

                position = processPositionList(list);

                bit += position.getEndIndexTarget() + 1;
                eit = bit + fixedSize;
                bir += position.getEndIndexRef() + 1;
                eir = bir + fixedSize;

                write(outFile, text, true);


                int difference = seq_target.length() - bit;

                if (difference <= kMerLength) {
                    text = (seq_target.substring(bit));
                    write(outFile, text, true);
                    break;
                } else if (difference < fixedSize) {
                    eit = seq_target.length() - 1;
                }

                int difference_ref = seq_reference.length() - bir;

                if (difference_ref < fixedSize) {
                    eir = seq_reference.length() - 1;
                }

                if (difference_ref <= kMerLength) {
                    text = (seq_target.substring(bit));
                    if (text.length() <= 0) {
                        break;
                    } else {
                        write(outFile, text, true);
                        break;
                    }
                }

                if (accept) {
                    text = (target.substring(position.getEndIndexTarget() + 1)) + "\n";
                    write(outFile, text, true);
                    bit = _eit;
                    eit = bit + fixedSize;
                    eir = __eir + fixedSize;
                    bir = __bir;
                }
                accept = false;
            }

            if (!traverse) {
                _iteration = 5;
                String enhanced_file_name = format(outFile);

                String reconstructedString = reconstruct(enhanced_file_name, seq_reference);

                int hd = calculateHD(reconstructedString, seq_target, Math.min(reconstructedString.length(), seq_target.length()));
                //System.out.println("HAMMING DISTANCE: " + hd);

                if (!reconstructedString.equalsIgnoreCase(seq_target)) {
                    System.out.println("OPS! NOT SAME...");
                    System.out.println("DIFFERENCE: " + (reconstructedString.length() - seq_target.length()));
                    if (seq_target.length() == reconstructedString.length()) {
                        int hammingDistance = calculateHD(reconstructedString, seq_target);
                        System.out.println("HAMMING DISTANCE: " + hammingDistance);
                    }

                    System.out.println("----------------------------------------------------------------------------");
                }

                reformat(enhanced_file_name, final_file, begin_index, end_index);

                file = new File(enhanced_file_name);
                file.delete();
                file = new File(outFile);
                file.delete();
            }
        }

        return reject;
    }

    public static String reconstruct(String inFileName, String reference) throws IOException {
        FileInputStream fileInputStream = new FileInputStream(inFileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        StringBuilder strBuf = new StringBuilder();
        while ((line = bufferedReader.readLine()) != null) {
            if (line.contains(",")) {
                String[] strings = line.split(",");
                int begin = Integer.parseInt(strings[0]);
                int end = Integer.parseInt(strings[1]);
                try {
                    String text = reference.substring(begin, end + 1);
                    strBuf.append(text);
                } catch (Exception x) {
                    x.getMessage();
                }
            } else if (line.length() > 0) {
                if (!line.contains("^")) {
                    strBuf.append(line);
                }
            }
        }
        bufferedReader.close();
        return strBuf.toString();
    }

    public static String format(String fileName) throws IOException {
        FileInputStream fileInputStream = new FileInputStream(fileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        StringBuilder strBuf = new StringBuilder();
        List<Integer> list = new ArrayList<Integer>();
        while ((line = bufferedReader.readLine()) != null) {
            if (line.contains(",")) {
                String[] strings = line.split(",");
                int begin = Integer.parseInt(strings[0]);
                int end = Integer.parseInt(strings[1]);
                if (list.size() > 0) {
                    int prevEnd = list.get(list.size() - 1);
                    if (begin != prevEnd + 1) {
                        strBuf.append(list.get(0)).append(",").append(list.get(list.size() - 1)).append("\n");
                        list = new ArrayList<Integer>();
                    }
                }
                list.add(begin);
                list.add(end);
            } else if (line.length() > 0) {
                try {
                    strBuf.append(list.get(0)).append(",").append(list.get(list.size() - 1)).append("\n");
                    if (!line.contains("^")) {
                        strBuf.append(line).append("\n");
                    }
                } catch (Exception x) {
                    if (!line.contains("^")) {
                        strBuf.append(line).append("\n");
                    }
                }
                list = new ArrayList<Integer>();
            }
        }
        if (list.size() > 0) {
            strBuf.append(list.get(0)).append(",").append(list.get(list.size() - 1)).append("\n");
        }
        bufferedReader.close();

        String outFileName = "out_enhanced.txt";

        File file = new File(outFileName);
        if (file.exists()) {
            file.delete();
        }
        write(outFileName, strBuf.toString(), true);
        return outFileName;
    }

    public static String reformat(String fileName, String final_file, int begin_index, int end_index) throws IOException {
        FileInputStream fileInputStream = new FileInputStream(fileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        StringBuilder strBuf = new StringBuilder();

        String pre_amble = "-> " + String.valueOf(begin_index) + " " + String.valueOf(end_index) + "\n";
        strBuf.append(pre_amble);

        List<Integer> list = new ArrayList<Integer>();
        List<String> stringList = new ArrayList<String>();

        while ((line = bufferedReader.readLine()) != null) {
            stringList.add(line);
        }

        int prev = 0;
        boolean accept = false;

        for (int i = 0; i < stringList.size(); i++) {
            String str = stringList.get(i);

            if (str.contains(",")) {
                if (!accept) {
                    String[] strings = str.split(",");
                    int begin = Integer.parseInt(strings[0]);
                    int end = Integer.parseInt(strings[1]);
                    list.add(begin);
                    list.add(end);
                    prev = end;
                    strBuf.append(list.get(0)).append(",").append(list.get(list.size() - 1)).append("\n");
                    accept = true;
                } else {
                    String[] strings = str.split(",");
                    int begin = Integer.parseInt(strings[0]);
                    int end = Integer.parseInt(strings[1]);
                    list.add(begin - prev);
                    list.add(end - begin);
                    prev = end;

                    strBuf.append(list.get(0)).append(",").append(list.get(list.size() - 1)).append("\n");

                }
                list = new ArrayList<Integer>();
            } else if (str.length() > 0) {
                strBuf.append(str).append("\n");
            }
        }

        if (list.size() > 0) {
            strBuf.append(list.get(0)).append(",").append(list.get(list.size() - 1)).append("\n");
        }

        strBuf.append("#\n");

        bufferedReader.close();

        write(final_file, strBuf.toString(), true);
        return final_file;
    }

    public static class Position {

        private int beginIndexRef;
        private int endIndexRef;
        private int beginIndexTarget;
        private int endIndexTarget;


        public int getBeginIndexRef() {
            return beginIndexRef;
        }

        public void setBeginIndexRef(int beginIndexRef) {
            this.beginIndexRef = beginIndexRef;
        }

        public int getEndIndexRef() {
            return endIndexRef;
        }

        public void setEndIndexRef(int endIndexRef) {
            this.endIndexRef = endIndexRef;
        }

        public int getBeginIndexTarget() {
            return beginIndexTarget;
        }

        public void setBeginIndexTarget(int beginIndexTarget) {
            this.beginIndexTarget = beginIndexTarget;
        }

        public int getEndIndexTarget() {
            return endIndexTarget;
        }

        public void setEndIndexTarget(int endIndexTarget) {
            this.endIndexTarget = endIndexTarget;
        }
    }

    public static class NewKMer {
        private String kMer;
        private int startPosition;

        public String getkMer() {
            return kMer;
        }

        public void setkMer(String kMer) {
            this.kMer = kMer;
        }

        public int getStartPosition() {
            return startPosition;
        }

        public void setStartPosition(int startPosition) {
            this.startPosition = startPosition;
        }

    }

}
