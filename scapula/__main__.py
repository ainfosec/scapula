import shoulder
import os

import scapula.generator

# Scapula-specific constants
scapula_pkg_dir = os.getcwd()
scapula_output_dir = os.path.join(scapula_pkg_dir, "output")

# Shoulder configs
shoulder.config.shoulder_output_dir = scapula_output_dir

# Main
regs = shoulder.parser.parse_registers(shoulder.config.xml_register_dir)
regs = shoulder.filters.apply_filters(regs)
shoulder.generator.generate_all(regs, shoulder.config.shoulder_output_dir)
