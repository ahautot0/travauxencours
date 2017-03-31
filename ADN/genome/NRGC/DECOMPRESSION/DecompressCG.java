import java.io.*;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class DecompressCG {

    public static List<Position> list;
    public static List<Position> next_list;
    public static String meta_data;
    public static int length;

    public DecompressCG() {
        list = new ArrayList<Position>();
        next_list = new ArrayList<Position>();
    }

    public static void run_7_zip(String archive_file_name) throws IOException, InterruptedException {

        System.out.println("RUNNING 7zip...");

        String command = "./7za e " + archive_file_name;
        System.out.println(command);

        String line;
        Process p = Runtime.getRuntime().exec(command);

        BufferedReader bri = new BufferedReader
                (new InputStreamReader(p.getInputStream()));

        BufferedReader bre = new BufferedReader
                (new InputStreamReader(p.getErrorStream()));

        while ((line = bri.readLine()) != null) {
            System.out.println(line);
        }
        bri.close();

        while ((line = bre.readLine()) != null) {
            System.out.println(line);
        }
        bre.close();

        p.waitFor();
        System.out.println("DONE...");
    }

    public static String readSequence(String sequenceFileName) throws IOException {
        FileInputStream fileInputStream = new FileInputStream(sequenceFileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        int count_new_line = 0;
        StringBuilder strBuf = new StringBuilder();
        while ((line = bufferedReader.readLine()) != null) {
            count_new_line++;
            if (!line.startsWith(">")) {
                strBuf.append(line);
            } else {
                System.out.println(line);
            }
        }
        System.out.println("LINES: " + count_new_line);
        return strBuf.toString();
    }

    public static String read_target(String sequenceFileName) throws IOException {
        FileInputStream fileInputStream = new FileInputStream(sequenceFileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        StringBuilder strBuf = new StringBuilder();
        while ((line = bufferedReader.readLine()) != null) {
            strBuf.append(line);
            strBuf.append("\n");
        }
        return strBuf.toString();
    }

    public static class Position {
        int starting_position;
        int ending_position;
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

    public static StringBuilder reconstruct(List<String> target_list, String reference, boolean set_aside) throws IOException {
        StringBuilder strBuf = new StringBuilder();
        int counter = 0, prev_end = 0, index = 0, prev = 0;
        boolean accept = false, next_accept = false, is_upper = false, is_lower = false;
        List<Position> list = new ArrayList<Position>();
        List<Position> next_list = new ArrayList<Position>();

        for (String line : target_list) {

            if (set_aside) {

                if (!accept) {
                    if (line.contains("-")) {
                        accept = true;
                        prev = 0;
                        continue;
                    }
                    if (index == 0) {
                        meta_data = line + "\n";
                    } else if (index == 1) {
                        length = Integer.parseInt(line);
                    } else if (index == 2) {
                        if (line.equals("U")) {
                            reference = reference.toUpperCase();
                            is_upper = true;
                            accept = true;
                        } else if (line.equals("L")) {
                            reference = reference.toLowerCase();
                            is_lower = true;
                            accept = true;
                        } else if (line.equals("u")) {
                            reference = reference.toUpperCase();
                            is_upper = true;
                        }
                    } else {
                        String[] strings = line.split(" ");
                        Position position = new Position();
                        position.starting_position = prev + Integer.parseInt(strings[0]);
                        position.ending_position = Integer.parseInt(strings[1]);
                        list.add(position);
                        prev = position.starting_position;
                    }
                    index++;
                    continue;
                }

                if (!next_accept) {
                    if (line.startsWith("_")) {
                        next_accept = true;

                        if (next_list.size() > 0) {
                            if (is_upper) {
                                reference = reference.replaceAll("N", "");
                            } else if (is_lower) {
                                reference = reference.replaceAll("n", "");
                            }
                        }

                        continue;
                    }
                    String[] strings = line.split(" ");
                    Position position = new Position();
                    position.starting_position = prev + Integer.parseInt(strings[0]);
                    position.ending_position = Integer.parseInt(strings[1]);
                    next_list.add(position);
                    prev = position.starting_position;
                    continue;
                }
            } else {
                if (line.startsWith("->")) {

                    String[] strs = line.split(" ");
                    int begin_index = Integer.parseInt(strs[1]);
                    int end_index = Integer.parseInt(strs[2]);

                    reference = reference.substring(begin_index, end_index);
                } else if (line.contains(",")) {
                    String[] strings = line.split(",");
                    int begin = Integer.parseInt(strings[0]);
                    int end = Integer.parseInt(strings[1]);

                    if (counter == 0) {
                        String text = reference.substring(begin, end + 1);
                        strBuf.append(text);
                        prev_end = end;
                        counter++;
                    } else {
                        begin += prev_end;
                        end += begin;
                        String text = reference.substring(begin, end + 1);
                        strBuf.append(text);
                        prev_end = end;
                    }
                } else if (line.length() > 0) {
                    if (!line.contains("^")) {
                        strBuf.append(line);
                    }
                }
            }
        }

        if (next_list.size() > 0) {
            DecompressCG.next_list = new ArrayList<Position>();
            DecompressCG.next_list.addAll(next_list);
        }

        if (list.size() > 0) {
            DecompressCG.list = new ArrayList<Position>();
            DecompressCG.list.addAll(list);
        }

        if (strBuf.length() > 0) {
            return strBuf;
        }
        return new StringBuilder(reference);
    }

    public static void write(String fileName, String text) throws IOException {
        FileWriter fileWriter = new FileWriter(fileName, false);
        BufferedWriter out = new BufferedWriter(fileWriter);
        out.write(text);
        out.close();
    }

    public static long getCpuTime() {
        ThreadMXBean bean = ManagementFactory.getThreadMXBean();
        return bean.isCurrentThreadCpuTimeSupported() ? bean.getCurrentThreadCpuTime() : 0L;
    }

    public static List<List<String>> get_data_from_compressed_file(String compressed_file_name) throws IOException {
        List<List<String>> list = new ArrayList<List<String>>();

        FileInputStream fileInputStream = new FileInputStream(compressed_file_name);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        boolean accept = true;

        List<String> interim_list = new ArrayList<String>();

        while ((line = bufferedReader.readLine()) != null) {
            if (accept) {

                interim_list.add(line);

                if (line.equals("_")) {
                    list.add(interim_list);
                    interim_list = new ArrayList<String>();
                    accept = false;
                }
            } else if (line.equals("#")) {

                if (interim_list.size() > 0) {
                    list.add(interim_list);
                }
                interim_list = new ArrayList<String>();
            } else {
                interim_list.add(line);
            }
        }

        return list;
    }

    public static String decompress_run(String reference_file, String compressed_file, String de_compressed_file) throws IOException {

        Date startDate = new Date();
        long startCpuTimeNano = getCpuTime();

        DecompressCG decompressCG = new DecompressCG();

        String genomic_sequence = readSequence(reference_file);

        StringBuilder reconstructed_string = new StringBuilder();
        StringBuilder final_string = new StringBuilder();

        List<List<String>> final_list = get_data_from_compressed_file(compressed_file);

        System.out.println("-----------> FINAL_LIST_SIZE: " + final_list.size());

        genomic_sequence = reconstruct(final_list.get(0), genomic_sequence, true).toString();

        for (int j = 1; j < final_list.size(); j++) {

            reconstructed_string = reconstruct(final_list.get(j), genomic_sequence, false);

            final_string.append(reconstructed_string);

        }

        System.out.println("LOWER_CASEPOSITION_LIST_SIZE: " + list.size());
        System.out.println("Ns_POSITION_LIST_SIZE: " + next_list.size());

        System.out.println("BEFORE ADDING Ns: " + final_string.length());

        StringBuilder interim = new StringBuilder();
        int index = 0, iterator = 0, total = 0;
        boolean accept = false;

        if (next_list.size() > 0) {
            for (Position position : next_list) {
                total += position.ending_position;
                while (true) {
                    if (iterator >= position.starting_position && iterator < (position.starting_position + position.ending_position)) {
                        interim.append('N');
                    } else if (iterator < position.starting_position && !accept) {
                        interim.append(final_string.charAt(index));
                        index = index + 1;
                        if (index >= final_string.length()) {
                            accept = true;
                        }
                    }
                    iterator = iterator + 1;
                    if (iterator >= position.starting_position + position.ending_position) {
                        break;
                    }
                }
            }

            System.out.println("AFTER ADDING Ns: " + interim.length());

            while (index < final_string.length()) {
                interim.append(final_string.charAt(index));
                index = index + 1;
            }

            System.out.println("TOTAL Ns: " + total);
            final_string = new StringBuilder(interim);
            System.out.println("FINAL SIZE: " + final_string.length());
        }


        System.out.println("LOWER_CHARACTER_LIST_SIZE: " + list.size());

        for (Position position : list) {
            for (int i = position.starting_position; i < (position.starting_position + position.ending_position); i++) {
                final_string.setCharAt(i, Character.toLowerCase(final_string.charAt(i)));
            }
        }

        int iteration = final_string.length() / length;

        System.out.println("ITERATION: " + iteration);

        reconstructed_string = new StringBuilder();
        reconstructed_string.append(final_string.charAt(0));

        for (int i = 1; i < final_string.length(); i++) {
            if (i % length == 0) {
                reconstructed_string.append("\n");
            }
            reconstructed_string.append(final_string.charAt(i));
        }

        Date endDate = new Date();
        long taskCpuTimeNano = getCpuTime() - startCpuTimeNano;

        double diff = endDate.getTime() - startDate.getTime();
        double diffSeconds = diff / 1000;
        double diffMinutes = diff / (60 * 1000);

        System.out.println("Time in seconds*: " + (double) taskCpuTimeNano / 1000000000.0 + " seconds.");
        System.out.println("Time in seconds: " + diffSeconds + " seconds.");
        System.out.println("Time in minutes: " + diffMinutes + " minutes.");
        System.out.println("----------------------------------------------------------------------------");

        reconstructed_string.append("\n");

        System.out.println("AFTER: " + reconstructed_string.length());
        String decompressed_data = meta_data + reconstructed_string.toString();
        write(de_compressed_file, decompressed_data);

        return decompressed_data;

    }

    public static void main(String[] args) throws IOException, InterruptedException {
        String reference_file = "E:\\SUBRATA\\DATA\\HG19\\chr20.fa\\chr20.fa";
        //String target_file = "E:\\SUBRATA\\KO131\\chr20.consensus_fasta\\chr20.consensus_fasta";
        String target_file = "E:\\SUBRATA\\DATA\\YH\\YH\\chr20.fa\\chr20.fa";
        String compressed_file = "C:\\Users\\Anamica\\Desktop\\FINAL_OUT.TXT";
        String de_compressed_file = "C:\\Users\\Anamica\\Desktop\\DECOMPRESS_OUT.TXT";
        //String target_file = "E:\\SUBRATA\\DATA\\HG19\\chr20.fa\\chr20.fa";
        String in_file_name = "";

        //String reference_file = "E:\\SUBRATA\\KO131\\chr20.consensus_fasta\\chr20.consensus_fasta";

        if (args != null && args.length != 3) {
            System.out.println("INCORRECT NUMBER OF PARAMETERS...");
        } else if (args != null) {
            for (int i = 0; i < args.length; i++) {
                if (i == 0) {
                    reference_file = args[0];
                } else if (i == 1) {
                    compressed_file = args[1];
                    in_file_name = args[1];
                    compressed_file = compressed_file.replace(".7z", "");
                } else if (i == 2) {
                    de_compressed_file = args[2];
                } else if (i == 3) {
                    target_file = args[3];
                }
            }
        }

        run_7_zip(in_file_name);

        String decompressed_data = decompress_run(reference_file, compressed_file, de_compressed_file);
        
        File file = new File(compressed_file);
        file.delete();


        /*String exact = read_target(target_file);

        System.out.println("CALCULATED_DECOMPRESSION_SIZE: " + decompressed_data.length());
        System.out.println("EXACT_DECOMPRESSION_SIZE: " + exact.length());

        if (exact.length() == decompressed_data.length()) {
            System.out.println("HAMMING_DISTANCE: " + get_hamming_distance(decompressed_data, exact));
        }*/
    }

    public static int get_hamming_distance(String target, String sequence) {
        int hamming_distance = 0;

        for (int i = 0; i < target.length(); i++) {
            if (target.charAt(i) != sequence.charAt(i)) {
                hamming_distance++;
            }
        }

        return hamming_distance;
    }
}
