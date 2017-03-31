import java.io.*;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Utilities {

    public static String genomic_sequence = "";
    public static List<Integer> position_list = new ArrayList<Integer>();

    public static List<Contig> restList;

    public static String readSequence(String sequenceFileName) throws IOException {
        System.out.println("READING SEQUENCE...");

        FileInputStream fileInputStream = new FileInputStream(sequenceFileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        StringBuilder sequence = new StringBuilder();

        while ((line = bufferedReader.readLine()) != null) {
            if (!line.startsWith(">")) {
                sequence.append(line);
            }
        }
        return sequence.toString();
    }

    public static List<String> readContigs(String contigsFileName) throws IOException {
        List<String> list = new ArrayList<String>();
        FileInputStream fileInputStream = new FileInputStream(contigsFileName);
        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            if (!line.contains("contig")) {
                list.add(line);
            }
        }
        return list;
    }

    public static List<Integer> positionsOfPattern(String text, String pattern, double probability) {
        List<Integer> positions = new ArrayList<Integer>();
        Pattern p = Pattern.compile(pattern);
        Matcher m = p.matcher(text);
        while (m.find()) {
            positions.add(m.start());
            /*if (!canNotBeAdded(probability)) {
                positions.add(m.start());
            } else {
                //System.out.println("MISSED...");
            }*/
        }
        return positions;
    }

    public static int count_occurrences(String text, String pattern) {
        Pattern p = Pattern.compile(pattern);
        Matcher m = p.matcher(text);
        int count = 0;
        while (m.find()) {
            count++;
        }
        return count;
    }

    public static boolean canNotBeAdded(double probability) {

        if (probability <= 0.0) {
            return false;
        }
        Random random = new Random();
        double randomNumber = random.nextDouble();
        return randomNumber <= probability;
    }

    public static void writeToDisk(String fileName, String text, boolean append) throws IOException {
        FileWriter fstream = new FileWriter(fileName, append);
        BufferedWriter out = new BufferedWriter(fstream);
        out.write(text);
        out.close();
    }

    public static void write(String fileName, String text, boolean append) throws IOException {
        FileWriter fileWriter = new FileWriter(fileName, append);
        BufferedWriter out = new BufferedWriter(fileWriter);
        out.write(text);
        out.close();
    }

    public static List<Double> getFragmentSizeList(String sequence, String pattern, double probability, double elongation) throws IOException {

        List<Integer> list = positionsOfPattern(sequence, pattern, probability);
        position_list.addAll(list);

        if (list.size() <= 1) {

            if (list.size() == 1) {
                Contig contig = new Contig();
                contig.setStartingPosition(list.get(0));
                restList.add(contig);
            }

            return null;
        }

        List<Double> fragmentSizeList = new ArrayList<Double>();
        fragmentSizeList.add((double) (list.get(0) + pattern.length()));
        if (elongation > 1) {
            Random random = new Random();
            for (int i = 1; i < list.size(); i++) {
                if (random.nextInt(2) == 1) {
                    double size = list.get(i) - list.get(i - 1);
                    double increaseBy = size * ((double) random.nextInt((int) elongation)) / 100;
                    size = size + increaseBy;
                    fragmentSizeList.add(size);
                } else {
                    double size = list.get(i) - list.get(i - 1);
                    double decreaseBy = size * ((double) random.nextInt((int) elongation)) / 100;
                    size = size - decreaseBy;
                    fragmentSizeList.add(size);
                }
            }
        } else {
            for (int i = 1; i < list.size(); i++) {
                double size = list.get(i) - list.get(i - 1);
                fragmentSizeList.add(size);
            }
        }

        return fragmentSizeList;
    }

    public static void createOpMap(String opFileName, List<Double> list) throws IOException {
        deleteFile(opFileName);
        StringBuilder text = new StringBuilder();
        for (int i = 0; i < list.size(); i++) {
            text.append(String.valueOf(list.get(i).intValue()) + "\n");
        }
        writeToDisk(opFileName, text.toString(), false);
    }

    public static void deleteFile(String fileName) {
        File file = new File(fileName);
        if (file.exists()) {
            file.delete();
        }
    }

    public static List<Double> readOpMap(String fileName) throws IOException {
        List<Double> list = new ArrayList<Double>();
        FileInputStream fileInputStream = new FileInputStream(fileName);

        DataInputStream dataInputStream = new DataInputStream(fileInputStream);
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInputStream));

        String strLine;

        while ((strLine = bufferedReader.readLine()) != null) {
            list.add((double) Integer.parseInt(strLine));
        }

        dataInputStream.close();
        return list;
    }

    public static int min(int a, int b, int c) {
        return (Math.min(Math.min(a, b), c));
    }

    public static List<String> createContigs(String sequence, int length) throws IOException {

        List<String> contigs = new ArrayList<String>();

        System.out.println("TARGET SIZE: " + sequence.length());

        int parts = sequence.length() / length;
        String contig = "";
        int start = 0, end = 0;

        for (int i = 0; i <= parts; i++) {
            if (i == 0) {
                end = length;
                contig = sequence.substring(start, end);
                contigs.add(contig);
            } else if (i < parts) {
                start = end;
                end = start + length;
                contig = sequence.substring(start, end);
                contigs.add(contig);
            } else {
                /*start = end;
                contig = sequence.substring(start);
                contigs.add(contig);*/

                start = end;
                contig = sequence.substring(start);
                contig = contigs.get(contigs.size() - 1) + contig;
                contigs.set((contigs.size() - 1), contig);
            }
        }
        return contigs;
    }

    public static void similarityCheck(String fileOne, String fileTwo) throws IOException {
        String s1 = readSequence(fileOne);
        List<String> list = readContigs(fileTwo);
        String s2 = "";
        for (String string : list) {
            s2 += string;
        }

        if (s1.equals(s2)) {
            System.out.println("HURRAY! SIMILAR...");
        } else {
            System.out.println("OH GOD! NOT SIMILAR...");
        }

        System.out.println(s1.length() + "    " + s2.length());
    }
}
