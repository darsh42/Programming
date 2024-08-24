class CopyConstructor {
    public static void main(String[] args) {
        Thing original = new Thing();
        Thing copy = new Thing(original);
        System.out.println("Original: " + original.getName());
        System.out.println("Copy: " + copy.getName());
    }
}

class Thing {
    public String name;
    public Thing() {
        name = "Thing";
    }

    public Thing(Thing toCopy) {
        this.name = toCopy.getName();
    }

    public String getName() {
        return name;
    }
}
