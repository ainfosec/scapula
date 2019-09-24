from typing import TextIO
from scapula.writer.formating import write_indent


def switch_to_el(outfile: TextIO, level: int, indent: int=0):
    """Writes a function to the given output file that changes the current
    exception level that Scapula OS is executing at.

    Args:
        outfile: The output file to be written
        level: The exception level to swtich to
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> switch_to_el(sys.stdout, 1)
        switch_to_el(1);
    """
    write_indent(outfile, indent)
    outfile.write("switch_to_el(" + str(level) + ");\n")


def get_current_el(outfile, var, indent=0):
    """Writes a function to the given output file that retrieves the current
    exception level that Scapula OS is executing at.

    Args:
        outfile: The output file to be written
        var: The name of the variable to read the result into
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> var = declare_variable(sys.stdout, "x", 64)
        >>> get_current_el(sys.stdout, var)
        uint64_t x = 0;
        x = get_current_el();
    """
    write_indent(outfile, indent)
    outfile.write(str(var) + " = get_current_el();\n")


def panic(outfile, indent=0):
    """Writes a function to the given output file that halts execution of
    Scapula OS and prints an error banner.

    Args:
        outfile: The output file to be written
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> panic(sys.stdout)
        panic();
    """
    write_indent(outfile, indent)
    outfile.write("panic();\n")
