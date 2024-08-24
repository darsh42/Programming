class SingletonPattern {
    public static void main(String[] args) {
        Database db = Database.getInstance("Shopping");
        Database bd = Database.getInstance("Not Shopping");
        System.out.println("Database: " + db.getName());
        System.out.println("Database: " + bd.getName());
    }
}

class Database {
    private String name;
    private static Database instance = null;

    private Database(String name) {
        this.name = name;
    }

    public static Database getInstance(String name) {
        if (instance == null) {
            instance = new Database(name);
        }

        return instance;
    }

    public String getName() {
        return name;
    }
}
