from typing import TextIO
from scapula.writer.formating import write_indent


def print_message(outfile: TextIO, msg: str, format_str: str="", indent: int=0):
    """Writes a print statement to the given output file.

    The generated print statement will print at run time regardless of the
    configured scapula OS log level

    Args:
        outfile: The output file to be written
        msg: A printf style message to be printed
        format_str (optional): A printf style format string for the message
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> print_message(sys.stdout, "Hello, Scapula OS!")
        SCAPULA_PRINT("Hello, Scapula OS!");
    """
    write_indent(outfile, indent)
    outfile.write("SCAPULA_PRINT(\"{msg}\"{fmt});\n".format(
        msg=str(msg),
        fmt="" if not format_str else ", " + str(format_str)
    ))


def print_error(outfile: TextIO, msg: str, format_str: str="", indent: int=0):
    """Writes an error print statement to the given output file.

    The generated error print statement will print at log level >= 1

    Args:
        outfile: The output file to be written
        msg: A printf style message to be printed
        format_str (optional): A printf style format string for the message
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> print_error(sys.stdout, "Error: %d", format_str="1")
        SCAPULA_ERROR("Error: %d", 1);
    """
    write_indent(outfile, indent)
    outfile.write("SCAPULA_ERROR(\"{msg}\"{fmt});\n".format(
        msg=str(msg),
        fmt="" if not format_str else ", " + str(format_str)
    ))


def print_warning(outfile: TextIO, msg: str, format_str: str="", indent: int=0):
    """Writes a warning print statement to the given output file.

    The generated warning print statement will print at log level >= 2

    Args:
        outfile: The output file to be written
        msg: A printf style message to be printed
        format_str (optional): A printf style format string for the message
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> print_warning(sys.stdout, "Warning: %d", format_str="1")
        SCAPULA_ALERT("Warning: %d", 1);
    """
    write_indent(outfile, indent)
    outfile.write("SCAPULA_ALERT(\"{msg}\"{fmt});\n".format(
        msg=str(msg),
        fmt="" if not format_str else ", " + str(format_str)
    ))


def print_info(outfile: TextIO, msg: str, format_str: str="", indent: int=0):
    """Writes an informational print statement to the given output file.

    The generated info print statement will print at log level >= 3

    Args:
        outfile: The output file to be written
        msg: A printf style message to be printed
        format_str (optional): A printf style format string for the message
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> print_info(sys.stdout, "Info: %d", format_str="1")
        SCAPULA_INFO("Info: %d", 1);
    """
    write_indent(outfile, indent)
    outfile.write("SCAPULA_INFO(\"{msg}\"{fmt});\n".format(
        msg=str(msg),
        fmt="" if not format_str else ", " + str(format_str)
    ))


def print_debug(outfile: TextIO, msg: str, format_str: str="", indent: int=0):
    """Writes a debug print statement to the given output file.

    The generated debug print statement will print at log level >= 4

    Args:
        outfile: The output file to be written
        msg: A printf style message to be printed
        format_str (optional): A printf style format string for the message
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> print_debug(sys.stdout, "Debug: %d", format_str="1")
        SCAPULA_DEBUG("Debug: %d", 1);
    """
    write_indent(outfile, indent)
    outfile.write("SCAPULA_DEBUG(\"{msg}\"{fmt});\n".format(
        msg=str(msg),
        fmt="" if not format_str else ", " + str(format_str)
    ))
