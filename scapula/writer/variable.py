from typing import TextIO
from scapula.writer.formating import write_indent


def declare_variable(outfile: TextIO, name: str, size: int, value: int=0,
                     const: bool=False, indent: int=0) -> str:
    """Declares a C-style variable in the given output file

    Args:
        outfile: The output file to be written
        name: The name of the variable to be declared
        size: The size of the variable to be declared
        value (optional): The initial value for the variable
        const (optional): True to declare variable const, False otherwise
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> declare_variable(sys.stdout, "x", 64)
        uint64_t x = 0;
    """
    write_indent(outfile, indent)

    size_type = "uint32_t"
    if size == 64:
        size_type = "uint64_t"
    elif size == 16:
        size_type = "uint16_t"
    elif size == 8:
        size_type = "uint8_t"

    output = "{const}{size_type} {name} = {val};\n".format(
        const="const " if const is True else "volatile ",
        size_type=size_type,
        name=str(name),
        val=hex(value) if value else "0"
    )

    outfile.write(output)

    return str(name)


def assign_variable(outfile: TextIO, var: str, rvalue: str, indent: int=0):
    """Writes a variable assignment statement using the given r-value expression

    Args:
        outfile: The output file to be written
        var: The name of the variable to be assigned
        rvalue: The r-value (expression) to be used in the variable assignment
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> var = declare_variable(sys.stdout, "x")
        >>> assign_variable(sys.stdout, var, "1 + 1")
        x = 1 + 1;
    """
    write_indent(outfile, indent)
    outfile.write(str(var) + " = " + str(rvalue) + ";\n")
