import java.io.IOException;
import java.util.*;

public class Assembly {

    public static int notMapped;
    public static int notMappedSize;
    public static int ranSd = 0;

    public static int getTotalContigSize(List<String> list) {
        int total = 0;
        for (String string : list) {
            total += string.length();
        }
        return total;
    }

    public static HashMap greedyScoringScheme(String opFileName,
                                              String contigsFileName, String pattern, double probability,
                                              double elongation) throws IOException {
        HashMap hashMap = new HashMap();
        List<String> contigsList = Utilities.readContigs(contigsFileName);
        int totalLength = getTotalContigSize(contigsList);

        System.out.println("REAL_CONTIGS: " + contigsList.size());
        System.out.println("TOTAL_LENGTH: " + totalLength);

        List<Double> opList = Utilities.readOpMap(opFileName);

        List<Contig> contigs = getMappedContigs(contigsList, pattern, probability, elongation);
        contigsList.clear();

        System.out.println("MAPPED_CONTIGS: " + contigs.size());

        int best = 0;
        int prev_list_size = 0;
        boolean passed = false;

        for (int i = 0; i < contigs.size(); i++) {

            double min = Double.MAX_VALUE;

            int slide = best;

            if (i % 50 == 0) {
                System.out.println("COMPLETED: " + i);
            }

            Contig contig = contigs.get(i);
            List<Double> conFragList = contig.getFragmentSizeList();

            int add = 0;
            int begin = slide + prev_list_size / 2;
            int end = begin + 3 * conFragList.size();

            if (i > 0 && !passed) {
                begin = 0;
                end = opList.size() - conFragList.size() + 1;
            }

            passed = false;

            if (end > opList.size() - conFragList.size() + 1) {
                end = opList.size() - conFragList.size() + 1;
            }

            for (int k = begin; k < end; k++) {

                int textPos = k + 1;
                int patternPos = 1;
                int missedRS = 1;
                int test = 0;
                double conFragSize = 0.0;
                double opFragSize = 0.0;
                double scoreOpt = 0.0;
                double scoreCon = 0.0;

                while (true) {
                    if (add == 1) {
                        conFragSize += conFragList.get(patternPos);
                    } else if (add == 0) {
                        conFragSize = conFragList.get(patternPos);
                        opFragSize = opList.get(textPos);
                    } else if (add == 2) {
                        opFragSize += opList.get(textPos);
                    }
                    double sd = conFragList.get(patternPos) * (double) ranSd / 100;
                    double lowerBound = opFragSize - sd;
                    double upperBound = opFragSize + sd;

                    if (conFragSize >= lowerBound && conFragSize <= upperBound) {
                        patternPos = patternPos + 1;
                        textPos = textPos + 1;
                        add = 0;
                    } else if (conFragSize < lowerBound) {
                        patternPos = patternPos + 1;
                        missedRS = missedRS + 1;
                        test++;
                        add = 1;
                    } else if (conFragSize > upperBound) {
                        missedRS = missedRS + 1;
                        textPos = textPos + 1;
                        add = 2;
                    }

                    if (test > conFragList.size()) break;

                    if (patternPos >= conFragList.size() || textPos >= opList.size()) {

                        Contig new_contig = new Contig();
                        new_contig.setContigId(contig.getContigId());
                        new_contig.setStartingPosition(Utilities.position_list.get(k));

                        if (add == 0) {
                            textPos = textPos - 1;
                            new_contig.setEndingPosition(textPos);
                        } else {
                            new_contig.setEndingPosition(textPos);
                        }

                        for (int index = k; index < textPos; index++) {
                            scoreOpt += Math.pow(opList.get(index), 1);
                        }

                        for (int index = 0; index < patternPos; index++) {
                            scoreCon += Math.pow(conFragList.get(index), 1);
                        }

                        double matchedScore = Math.abs(scoreCon - scoreOpt) + 9999 * missedRS;

                        new_contig.setMatchedScore(matchedScore);

                        if (min >= matchedScore) {
                            min = matchedScore;
                            best = k;
                        }

                        List<Contig> list = (List<Contig>) hashMap.get(contig.getContigId());

                        if (list == null) {
                            list = new ArrayList<Contig>();
                            list.add(new_contig);
                            hashMap.put(contig.getContigId(), list);
                            passed = true;
                        } else {
                            list.add(new_contig);
                            hashMap.put(contig.getContigId(), list);
                        }
                        break;
                    }
                }
            }

            if (!passed) {
                System.out.println("NOT_PASSED: " + i);
                System.exit(0);
                i = i - 1;
            }
            prev_list_size = conFragList.size();
        }
        return hashMap;
    }

    public static List<Contig> getMappedContigs(List<String> contigsList, String pattern,
                                                double probability, double elongation) throws IOException {
        int counter = 0;
        List<Contig> list = new ArrayList<Contig>();
        Utilities.restList = new ArrayList<Contig>();

        for (String string : contigsList) {
            List<Double> orderedFragmentSizeList = Utilities.getFragmentSizeList(string, pattern, probability, elongation);

            if (orderedFragmentSizeList != null && orderedFragmentSizeList.size() > 0) {

                Contig contig = new Contig();
                contig.setContigId(counter);
                contig.setFragmentSizeList(orderedFragmentSizeList);
                list.add(contig);
            } else {
                notMapped++;
                notMappedSize += string.length();
            }
            counter++;
        }
        return list;
    }

    public static List<Contig> getSortedList(List<Contig> list) {
        List<Contig> sortedList = new ArrayList<Contig>();

        Contig[] values = new Contig[list.size()];
        int index = 0;
        for (Contig contig : list) {
            values[index] = contig;
            index++;
        }

        Arrays.sort(values);
        for (int i = 0; i < list.size(); i++) {
            sortedList.add(values[i]);
        }
        return sortedList;
    }

    public static List<Contig> greedyScheduleScheme(String opFileName,
                                                    String conFileName, String pattern, int depth,
                                                    double probability, double elongation) throws IOException {
        List<Contig> usedContigs = new ArrayList<Contig>();

        HashMap hashMap = greedyScoringScheme(opFileName, conFileName, pattern, probability, elongation);

        System.out.println("HASH_MAP_SIZE: " + hashMap.size());

        Set set = hashMap.entrySet();
        Iterator iterator = set.iterator();

        while (iterator.hasNext()) {

            Map.Entry me = (Map.Entry) iterator.next();
            List<Contig> list = (List<Contig>) me.getValue();
            List<Contig> sortedList = getSortedList(list);

            Contig contig = getNextContig(sortedList, usedContigs, depth);

            if (contig != null) {
                usedContigs.add(contig);
            }

            //usedContigs.add(sortedList.get(0));
            list.clear();
        }

        hashMap.clear();

        return usedContigs;
    }

    public static boolean isOverlapped(List<Contig> contigs, Contig contig) {
        for (Contig con : contigs) {
            int startingPosition = con.getStartingPosition();
            int endPosition = con.getEndingPosition();
            if (contig.getStartingPosition() >= startingPosition && contig.getStartingPosition() <= endPosition) {
                return true;
            } else if (contig.getEndingPosition() >= startingPosition && contig.getEndingPosition() <= endPosition) {
                return true;
            } else if (contig.getStartingPosition() <= startingPosition && contig.getEndingPosition() >= endPosition) {
                return true;
            } else if (contig.getStartingPosition() <= startingPosition &&
                    (contig.getEndingPosition() >= startingPosition && contig.getEndingPosition() <= endPosition)) {
                return true;
            }
        }
        return false;
    }

    public static Contig getNextContig(List<Contig> sortedList, List<Contig> usedContigs, int depth) {
        for (int i = 0; i < sortedList.size(); i++) {
            Contig contig = sortedList.get(i);
            boolean overlapped = isOverlapped(usedContigs, contig);
            if (!overlapped) {
                return contig;
            }

            if (i >= depth) break;
        }
        return null;
    }

    public static String getRandomPattern(int size) {
        String pattern = "";
        Random rand = new Random();
        for (int i = 0; i < size; i++) {
            int randomNumber = rand.nextInt(4);
            if (randomNumber == 0) {
                pattern += "a";
            } else if (randomNumber == 1) {
                pattern += "g";
            } else if (randomNumber == 2) {
                pattern += "c";
            } else if (randomNumber == 3) {
                pattern += "t";
            }
        }
        return pattern;
    }

}
