import java.util.List;

public class Contig implements Comparable {

    private int contigId;
    private List<Double> fragmentSizeList;
    private double matchedScore;
    private int startingPosition;
    private int endingPosition;

    public int getStartingPosition() {
        return startingPosition;
    }

    public void setStartingPosition(int startingPosition) {
        this.startingPosition = startingPosition;
    }

    public int getEndingPosition() {
        return endingPosition;
    }

    public void setEndingPosition(int endingPosition) {
        this.endingPosition = endingPosition;
    }

    public int getContigId() {
        return contigId;
    }

    public void setContigId(int contigId) {
        this.contigId = contigId;
    }

    public List<Double> getFragmentSizeList() {
        return fragmentSizeList;
    }

    public void setFragmentSizeList(List<Double> fragmentSizeList) {
        this.fragmentSizeList = fragmentSizeList;
    }

    public double getMatchedScore() {
        return matchedScore;
    }

    public void setMatchedScore(double matchedScore) {
        this.matchedScore = matchedScore;
    }

    public int compareTo(Object obj) {
        Contig tmp = (Contig) obj;
        if ((this.matchedScore) < (tmp.matchedScore)) {
/* instance lt received */
            return -1;
        } else if ((this.matchedScore) > (tmp.matchedScore)) {
/* instance gt received */
            return 1;
        }
/* instance == received */
        return 0;
    }
}
