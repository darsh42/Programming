import sys
import os

def main() -> int:
    if (len(sys.argv) != 2):
        print("Usage: python GenerateAst.py <output_dir>")
        return 64

    output_dir: str = sys.argv[1]
    gen_expr = GenerateAst(output_dir, "Expr", (
            "Binary : Expr left, Token operator, Expr right",
            "Grouping : Expr expression",
            "Literal : Object value",
            "Unary : Token operator, Expr right"));

    gen_stmt = GenerateAst(output_dir, "Stmt",(
        "Expression : Expr expression",
        "Print : Expr expression"
    ));

    gen_expr.AST()
    gen_stmt.AST()


class GenerateAst:
    def __init__(self, output_dir: str, base_name: str, types: list):
        self.output_dir = output_dir
        self.base_name  = base_name
        self.types      = types

    def AST(self):
        path: str = os.path.join(self.output_dir, self.base_name + ".java")
        with open(path, "x") as file:
            file.write("package Lox;\n")
            file.write("\n")
            file.write("import java.util.List;\n")
            file.write("\n")
            file.write("abstract class " + self.base_name + " {\n")

            self.define_visitor(file)

            for type in self.types:
                class_name: str = type.split(":")[0].replace(" ", "")
                fields: str = type.split(":")[1]
                self.defineType(file, class_name, fields)

            file.write("\n")
            file.write("    abstract <R> R accept(Visitor<R> visitor);\n")

            file.write("}\n")
            file.write("\n")

    def defineType(self, file, class_name: str, fieldList: str) -> None:
        file.write("\n")
        file.write("    static class " + class_name + " extends " + self.base_name + " {\n")


        file.write("        " + class_name + "(" + fieldList + ") {\n")

        fields: list = fieldList.split(",")
        for field in fields:
            name: str = field.split(" ")[2]
            file.write("            this." + name + " = " + name + ";\n")

        file.write("        }\n")

        file.write("\n")
        file.write("        @Override\n")
        file.write("        <R> R accept(Visitor<R> visitor) {\n")
        file.write("            return visitor.visit" + class_name + self.base_name + "(this);\n")
        file.write("        }\n")

        file.write("\n")
        for field in fields:
            file.write("        final " + field + ";\n")

        file.write("    }\n")

    def define_visitor(self, file) -> None:
        file.write("    interface Visitor<R> {\n")

        for type in self.types:
            type_name: str = type.split(":")[0].replace(" ", "")
            file.write("        R visit" + type_name + self.base_name + "(" + type_name + " " + self.base_name.lower() + ");\n")

        file.write("    }\n")

if __name__ == '__main__':
    main()
