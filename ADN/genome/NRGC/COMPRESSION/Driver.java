import java.io.*;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.*;

public class Driver {

    public static int division_length = 0;
    public static String _pattern = "";
    public static boolean compute_pattern = false;
    public static int pattern_length = 0;

    public static long getCpuTime() {
        ThreadMXBean bean = ManagementFactory.getThreadMXBean();
        return bean.isCurrentThreadCpuTimeSupported() ? bean.getCurrentThreadCpuTime() : 0L;
    }

    public Driver(String file_name) throws IOException {

        FileInputStream fileInputStream = new FileInputStream(file_name);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        int counter = 1;

        while ((line = bufferedReader.readLine()) != null) {
            if (counter == 2) {
                division_length = Integer.parseInt(line);
            } else if (counter == 4) {
                _pattern = line;
            } else if (counter == 6) {
                compute_pattern = !line.equalsIgnoreCase("off");
            } else if (counter == 8) {
                pattern_length = Integer.parseInt(line);
            }
            counter++;
        }

        bufferedReader.close();
    }

    public static void run_7_zip(String archive_file_name, String compressed_file_name) throws IOException, InterruptedException {

        System.out.println("RUNNING 7zip...");

        String command = "./7za a -t7z " + archive_file_name + " " + compressed_file_name;
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


    public static void main(String[] args) throws IOException, InterruptedException {

        long startCpuTimeNano = getCpuTime();

        Driver driver = new Driver("properties.prop");

        Assembly assembly = new Assembly();

        String reference_file_name = "E:\\SUBRATA\\DATA\\HG19\\chr20.fa\\chr20.fa";
        String target_file_name = "E:\\SUBRATA\\DATA\\YH\\YH\\chr20.fa\\chr20.fa";

        String conFileName = "C:\\Users\\Anamica\\Desktop\\contigs.fa";

        String opFileName = "C:\\Users\\Anamica\\Desktop\\op_map.txt";

        String final_file = "C:\\Users\\Anamica\\Desktop\\FINAL_OUT.TXT";

        if (args != null && args.length != 3) {
            System.out.println("INCORRECT NUMBER OF PARAMETERS...");
        } else if (args != null) {
            opFileName = "op_map.txt";
            conFileName = "contigs.fa";

            for (int i = 0; i < args.length; i++) {
                if (i == 0) {
                    reference_file_name = args[0];
                } else if (i == 1) {
                    target_file_name = args[1];
                } else if (i == 2) {
                    final_file = args[2];
                }
            }
        }

        int best = Integer.MIN_VALUE;

        Utilities.genomic_sequence = Utils.readSequence(reference_file_name);
        System.out.println("REFERENCE_LENGTH: " + Utilities.genomic_sequence.length());
        String seq_target = Utils.readSequence(target_file_name);
        System.out.println("TARGET_LENGTH: " + seq_target.length());

        boolean is_doable = false;
        double percent_align = 0.0;
        int target_length = seq_target.length();
        int reference_length = Utilities.genomic_sequence.length();

        if (reference_length > target_length) {
            percent_align = ((double) target_length / reference_length) * 100.0;
            if (percent_align > 95.00) {
                is_doable = true;
            }
        } else {
            percent_align = ((double) reference_length / target_length) * 100;
            if (percent_align > 95.00) {
                is_doable = true;
            }
        }

        System.out.println("%ALIGN: " + percent_align);

        int type = 0;

        if (Utils.is_upper_case(seq_target)) {
            type = 1;
            System.out.println("UPPER CHARACTER SETS...");
            Utilities.genomic_sequence = Utilities.genomic_sequence.toUpperCase();
            String prefix = Utils.meta_data + "\n" + Utils.length + "\n" + "U" + "\n";
            Utils.write(final_file, prefix, false);
        } else if (Utils.is_lower_case(seq_target)) {
            type = 2;
            System.out.println("LOWER CHARACTER SETS...");
            Utilities.genomic_sequence = Utilities.genomic_sequence.toLowerCase();
            String prefix = Utils.meta_data + "\n" + Utils.length + "\n" + "L" + "\n";
            Utils.write(final_file, prefix, false);

        } else {
            type = 3;
            System.out.println("MIXED CHARACTER SETS...");
            String prefix = Utils.meta_data + "\n" + Utils.length + "\n" + "u" + "\n";
            String suffix = "-" + "\n";
            Utilities.genomic_sequence = Utilities.genomic_sequence.toUpperCase();
            List<Utils.Position> list = Utils.get_lower_letters_position_list(seq_target);
            System.out.println("LOWER LIST SIZE: " + list.size());
            System.out.println("UPPER LIST SIZE: " + Utils.get_upper_letters_position_list(seq_target).size());
            Utils.write(final_file, list, false, prefix, suffix);
            seq_target = seq_target.toUpperCase();
        }

        List<Utils.Position> position_list = new ArrayList<Utils.Position>();

        if (is_doable) {
            position_list = Utils.get_position_list(seq_target);
        }

        System.out.println("POSITION LIST SIZE: " + position_list.size());

        if (position_list.size() > 0) {
            if (type == 1) {
                Utilities.genomic_sequence = Utilities.genomic_sequence.replaceAll("N", "");
                seq_target = seq_target.replaceAll("N", "");
            } else if (type == 2) {
                Utilities.genomic_sequence = Utilities.genomic_sequence.replaceAll("n", "");
                seq_target = seq_target.replaceAll("n", "");
            } else {
                Utilities.genomic_sequence = Utilities.genomic_sequence.replaceAll("N", "");
                seq_target = seq_target.replaceAll("N", "");
            }
        }

        System.out.println("MOD_REFERENCE_LENGTH: " + Utilities.genomic_sequence.length());
        System.out.println("MOD_TARGET_LENGTH: " + seq_target.length());

        String suffix = "_" + "\n";
        Utils.write(final_file, position_list, true, "", suffix);

        String pattern = "";
        String best_pattern = "";

        if (compute_pattern) {

            for (int i = 0; i < 70; i++) {
                pattern = Assembly.getRandomPattern(pattern_length);

                if (type == 1 || type == 3) {
                    pattern = pattern.toUpperCase();
                }

                int occurrences = Utilities.count_occurrences(Utilities.genomic_sequence, pattern);

                if (best <= occurrences) {
                    best = occurrences;
                    best_pattern = pattern;
                }
            }
            pattern = best_pattern;

        } else {

            if (type == 1 || type == 3) {
                pattern = _pattern.toUpperCase();
                //pattern = "ACTAC";
            } else {
                pattern = _pattern.toLowerCase();
                //pattern = "actac";
            }
        }

        int depth = 60;
        double probability = 0.0;
        double elongation = 1.0;
        Assembly.ranSd = 50;
        int length = division_length;

        System.out.println(division_length + "  " + pattern + "  " + compute_pattern + "  " + pattern_length);

        int decision = 1;

        Date startTime = new Date();

        List<String> _contigs = new ArrayList<String>();

        if (decision == 1) {
            System.out.println("READING SEQUENCE... ");
            System.out.println("SEQUENCE SIZE: " + Utilities.genomic_sequence.length());
            System.out.println("LISTING SIZE... ");
            List<Double> fragList = Utilities.getFragmentSizeList(Utilities.genomic_sequence, pattern, -1, 1);

            System.out.println("CREATING OP_MAP FILE... ");
            Utilities.createOpMap(opFileName, fragList);

            System.out.println("CREATING CONTIGS FILE... ");

            _contigs = Utilities.createContigs(seq_target, length);

            FileWriter fstream = new FileWriter(conFileName, false);
            BufferedWriter out = new BufferedWriter(fstream);

            for (int counter = 0; counter < _contigs.size(); counter++) {
                String contig = _contigs.get(counter);
                String text = ">contig_" + counter + " len=" + contig.length() + "\n" + contig + "\n";
                out.write(text);
            }

            out.close();
        }

        if (decision == 1) {

            System.out.println("GREEDY_SCHEDULE_SCHEME...");

            List<Contig> list = Assembly.greedyScheduleScheme(opFileName, conFileName, pattern, depth, probability, elongation);

            System.out.println("REALLY_MAPPED_CONTIGS: " + list.size());
            System.out.println("NOT_MAPPED: " + Assembly.notMapped);

            List<Position> posWiseList = getPositionList(list);
            System.out.println(posWiseList.size());

            getContigs(posWiseList, _contigs, pattern, final_file);

        }

        String compressed_file_name = final_file;
        String archive_file_name = compressed_file_name + ".7z";


        run_7_zip(archive_file_name, compressed_file_name);

        long taskCpuTimeNano = getCpuTime() - startCpuTimeNano;

        Date endTime = new Date();
        double diff = endTime.getTime() - startTime.getTime();
        double diffSeconds = diff / 1000;
        double diffMinutes = diff / (60 * 1000);
        double diffHours = diff / (60 * 60 * 1000);
        System.out.println("Time in seconds: " + (double) taskCpuTimeNano / 1000000000.0 + " seconds.");
        System.out.println("Time in seconds->: " + diffSeconds + " seconds.");
        System.out.println("Time in minutes: " + diffMinutes + " minutes.");
        System.out.println("Time in hours: " + diffHours + " hours.");
        System.out.println("----------------------------------------------------------------------------------------------------------------------------------");

        File file = new File(opFileName);
        file.delete();

        file = new File(conFileName);
        file.delete();
        
        file = new File(compressed_file_name);
        file.delete();
    }


    public static List<Position> getSortedPWList(List<Position> list) {
        List<Position> sortedList = new ArrayList<Position>();

        Position[] values = new Position[list.size()];
        int index = 0;
        for (Position position : list) {
            values[index] = position;
            index++;
        }

        Arrays.sort(values);
        sortedList.addAll(Arrays.asList(values).subList(0, list.size()));
        return sortedList;
    }

    public static List<Position> getPositionList(List<Contig> list) {

        List<Position> poList = new ArrayList<Position>();
        for (Contig contig : list) {
            Position position = new Position();
            position.setStartingPosition(contig.getStartingPosition());
            position.setContig(contig);
            poList.add(position);
        }

        return getSortedPWList(poList);
    }

    public static void getContigs(List<Position> pWList, List<String> _contigs, String pattern, String final_file) throws IOException {

        int counter = 0;
        double prev_percent = 0.0, percent = 0.0;

        for (Position aPWList : pWList) {

            String contig = _contigs.get(aPWList.getContig().getContigId());
            int index = contig.indexOf(pattern);

            if (index >= 0) {

                int begin_index = aPWList.getStartingPosition() - index;
                int end_index = begin_index + contig.length();

                if (end_index > Utilities.genomic_sequence.length()) {
                    end_index = Utilities.genomic_sequence.length();
                }

                if (begin_index < 0) {
                    begin_index = 0;
                }

                String reference = Utilities.genomic_sequence.substring(begin_index, end_index);
                boolean reject = Utils.run(reference, contig, final_file, begin_index, end_index);

                if (reject) {
                    counter++;
                }

                percent = ((double) counter / pWList.size()) * 100.0;

                if (percent != prev_percent) {
                    System.out.println("PERCENT: " + percent);
                }

                if (percent > 6.0) {
                    System.out.println("SYSTEM EXITING...");
                    System.exit(0);
                }

            } else {
                index = 0;
                String target = contig.substring(index);
                String reference = Utilities.genomic_sequence.substring(aPWList.getStartingPosition(), aPWList.getStartingPosition() + target.length());

                System.out.println(target.length() + " -> " + get_hamming_distance(target, reference));

                System.exit(0);

                System.out.println("--------------------------oooooooooo---------------------------------");
            }

            prev_percent = percent;
        }
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
