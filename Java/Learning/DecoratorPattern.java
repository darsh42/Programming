class DecoratorPattern {
    public static void main(String[] args) {
        TerminalPrinter tp = new TerminalPrinter();
        PrefixDecorator pd = new PrefixDecorator(tp, "> ");
        PostfixDecorator dp = new PostfixDecorator(pd, "~");
        System.out.println("TerminalPrinter: " + tp.getStream());
        System.out.println("PrefixDecorator: " + pd.getStream());
        System.out.println("PostfixDecorator: " + dp.getStream());
    }
}

class Printer {
    protected String printStream;
    public Printer() {
        printStream = "Default";
    }

    public String getStream() {
        return printStream;
    }
}

class TerminalPrinter extends Printer {
    public TerminalPrinter() {
        this.printStream = "stdout";
    }
}

class PrefixDecorator extends Printer {
    private Printer printer;
    private String prefix;

    public PrefixDecorator(Printer printer, String prefix) {
        this.printer = printer;
        this.prefix = prefix;
    }

    public String getStream() {
        return prefix + printer.getStream();
    }
}

class PostfixDecorator extends Printer {
    private Printer printer;
    private String postfix;

    public PostfixDecorator(Printer printer, String prefix) {
        this.printer = printer;
        this.postfix = prefix;
    }

    public String getStream() {
        return printer.getStream() + postfix;
    }
}
