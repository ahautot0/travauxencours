public class Position implements Comparable{

    private Contig contig;
    private int startingPosition;

    public Contig getContig() {
        return contig;
    }

    public void setContig(Contig contig) {
        this.contig = contig;
    }

    public int getStartingPosition() {
        return startingPosition;
    }

    public void setStartingPosition(int startingPosition) {
        this.startingPosition = startingPosition;
    }

    public int compareTo(Object obj) {
        Position tmp = (Position) obj;
        if ((this.startingPosition) < (tmp.startingPosition)) {
/* instance lt received */
            return -1;
        } else if ((this.startingPosition) > (tmp.startingPosition)) {
/* instance gt received */
            return 1;
        }
/* instance == received */
        return 0;
    }
}
