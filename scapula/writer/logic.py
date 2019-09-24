from typing import TextIO
from scapula.writer.formating import write_indent


def if_statement(outfile: TextIO, condition: str, indent: int=0):
    """Writes an if statement to the given output file.

    The given condition will be the contents of the if statement. Use endif()
    to close the written if statement

    Args:
        outfile: The output file to be written
        condition: The logical condition to be written
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> if_statment(sys.stdout, "x == 1")
        >>> endif(sys.stdout)
        if (x == 1) {
        }
    """
    write_indent(outfile, indent)
    outfile.write("if(" + str(condition) + ") {\n")


def else_if_statement(outfile: TextIO, condition: str, indent: int=0):
    """Writes an else if statement to the given output file.

    First closes the contents of a previously generated if statement. The given
    condition will then be written as the contents of the else if statement.
    Use endif() to close the written else if statement

    Args:
        outfile: The output file to be written
        condition: The logical condition to be written
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> if_statment(sys.stdout, "x == 1")
        >>> else_if_statment(sys.stdout, "x == 2")
        >>> endif(sys.stdout)
        if (x == 1) {
        }
        else if (x == 2) {
        }
    """
    write_indent(outfile, indent)
    outfile.write("}\n")
    write_indent(outfile, indent)
    outfile.write("else ")
    if_statement(outfile, condition, indent)


def else_statement(outfile: TextIO, indent: int=0):
    """Writes an else statement to the given output file.

    First closes the contents of a previously generated if statement, then
    writes an else statement. Use endif() to close the written else statement

    Args:
        outfile: The output file to be written
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> if_statment(sys.stdout, "x == 1")
        >>> else_statment(sys.stdout)
        >>> endif(sys.stdout)
        if (x == 1) {
        }
        else {
        }
    """
    write_indent(outfile, indent)
    outfile.write("}\n")
    write_indent(outfile, indent)
    outfile.write("else {\n")


def endif(outfile: TextIO, indent: int=0):
    """Closes a series of if, if else, and else statements

    Writes a closing bracket '}' and a newline character to the given outfile

    Args:
        outfile: The output file to be written
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> if_statment(sys.stdout, "x == 1")
        >>> endif(sys.stdout)
        if (x == 1) {
        }
    """
    write_indent(outfile, indent)
    outfile.write("}\n")
