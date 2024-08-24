class ConstructorChaining {
    public static void main(String[] args) {
        SmallThing s = new SmallThing();
    }
}

class BigThing {
    public BigThing() {
        System.out.println("I am Big thing");
    }
}

class MediumThing extends BigThing {
    public MediumThing() {
        System.out.println("I am Medium thing");
    }
}

class SmallThing extends MediumThing {
    public SmallThing() {
        System.out.println("I am Small thing");
    }
}
