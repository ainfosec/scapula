from typing import TextIO
from typing import Union
from shoulder.model.register import Register
from shoulder.model.field import Field
from scapula.writer.formating import write_indent


def get_register(outfile: TextIO, reg: Register, var: str, indent: int=0):
    """Writes a function to the given output file that reads the given register.

    The generated function reads given register reg into a variable named var.
    Variable var must be previously declared in the output file.

    Args:
        outfile: The output file to be written
        reg: The register to be read
        var: The name of the variable to place the result of the read into
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> reg = shoulder.model.register.Register(name="example", size=64, arch="aarch64")
        >>> var = declare_variable(sys.stdout, "x", reg.size)
        >>> get_register(sys.stdout, reg, var)
        uint64_t x = 0;
        x = aarch64_example_get();
    """
    write_indent(outfile, indent)

    output = "{var} = {es}_{rname}_get();\n".format(
        var=str(var),
        es=reg.execution_state if reg.execution_state else "external",
        rname=reg.name.lower()
    )

    outfile.write(output)


def set_register(outfile: TextIO, reg: Register, val: Union[str, int], indent: int=0):
    """Writes a function to the given output file that sets a value to a register.

    The generated function sets the given register reg with the given value val.

    Args:
        outfile: The output file to be written
        reg: The register to be written
        val: The value to be written to the register
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> reg = shoulder.model.register.Register(name="example", size=64, arch="aarch64")
        >>> set_register(sys.stdout, reg, 0xcafe)
        aarch64_example_set(0xcafe);
    """
    write_indent(outfile, indent)

    output = "{es}_{rname}_set({val});\n".format(
        es=reg.execution_state if reg.execution_state else "external",
        rname=reg.name.lower(),
        val=str(val) if type(val) == str else hex(val)
    )

    outfile.write(output)


def get_field(outfile: TextIO, reg: Register, field: Field, var: str, indent: int=0):
    """Writes a function to the given output file that reads the given register field

    The generated function reads the given field of the given register reg into
    a variable named var. Variable var must be previously declared in the
    output file.

    Args:
        outfile: The output file to be written
        reg: The register to be read
        field: The field to be read
        var: The name of the variable to place the result of the read into
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> reg = shoulder.model.register.Register(name="example", size=64, arch="aarch64")
        >>> field = shoulder.model.field.Field(name="field_1", msb=32, lsb=0)
        >>> var = declare_variable(sys.stdout, "x", reg.size)
        >>> get_field(sys.stdout, reg, field, var)
        uint64_t x = 0;
        x = aarch64_example_field_1_get();
    """
    write_indent(outfile, indent)

    output = "{var} = {es}_{rname}_{fname}_{getter}();\n".format(
        var=str(var),
        es=reg.execution_state if reg.execution_state else "external",
        rname=reg.name.lower(),
        fname=field.name.lower(),
        getter="is_enabled" if field.lsb == field.msb else "get"
    )

    outfile.write(output)


def set_field(outfile: TextIO, reg: Register, field: Field, val: int, indent: int=0):
    """Writes a function to the given output file that sets a value to a register field

    The generated function sets the given field in the given register with the
    given value val.

    Args:
        outfile: The output file to be written
        reg: The register to be written
        field: The field to be written
        val: The value to be written to the register field
        indent (optional): The number of leading tab characters to be written

    Examples:
        >>> reg = shoulder.model.register.Register(name="example", size=64, arch="aarch64")
        >>> field = shoulder.model.field.Field(name="field_1", msb=32, lsb=0)
        >>> set_field(sys.stdout, reg, field, 0xcafe)
        aarch64_example_field_1_set(0xcafe);
    """
    write_indent(outfile, indent)

    if field.msb == field.lsb:
        if val == 0:
            setter = "disable()"
        else:
            setter = "enable()"
    else:
        setter = "set(" + str(hex(val)) + ")"

    output = "{es}_{rname}_{fname}_{setter};\n".format(
        es=reg.execution_state if reg.execution_state else "external",
        rname=reg.name.lower(),
        fname=field.name.lower(),
        setter=setter
    )

    outfile.write(output)
