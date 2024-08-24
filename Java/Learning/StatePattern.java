class StatePattern {
    public static void main(String[] args) {
        Document doc = new Document("Hello.txt");
        for (int i = 0; i < 3; i++) {
            System.out.println("Document: " + doc.getState());
            doc.nextState();
        }
    }
}

class Document {
    private State state;
    protected String name;

    public Document(String name) {
        this.name = name;
        this.state = new Draft(this);
    }

    public String getState() {
        return state.getState();
    }

    public void nextState() {
        state = state.nextState();
    }
}

abstract class State {
    protected String state;
    protected Document document;

    public State(Document document) {
        this.document = document;
    }

    public String getState() {
        return this.state;
    }

    public abstract State nextState();

}

class Draft extends State {
    public Draft(Document doc) {
        super(doc);
        this.state = "Draft";
    }

    public State nextState() {
        return new Moderation(this.document);
    }
}

class Moderation extends State {
    public Moderation(Document doc) {
        super(doc);
        this.state = "Moderation";
    }

    public State nextState() {
        return new Published(this.document);
    }
}

class Published extends State {
    public Published(Document doc) {
        super(doc);
        this.state = "Published";
    }

    public State nextState() {
        return new Published(this.document);
    }
}
