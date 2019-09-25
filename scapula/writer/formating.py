from typing import TextIO

def write_indent(outfile: TextIO, count: int=1):
    """Writes a tab character to the given output file

    Args:
        outfile: The output file to be written
        count: The number of tab characters to be written
    """
    for i in range(0, count):
        outfile.write("\t")


def write_newline(outfile: TextIO, count:int=1):
    """Writes a newline character to the given output file

    Args:
        outfile: The output file to be written
        count: The number of newline characters to be written
    """
    for i in range(0, count):
        outfile.write("\n")
