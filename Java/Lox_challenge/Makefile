JFLAGS = -g -d build
JC = javac

CLASSES = Lox.java\
	  Scanner.java\
	  TokenType.java\
	  Token.java

default: classes

classes:
	$(JC) $(JFLAGS) $(CLASSES)

clean:
	$(RM) *.class
