import os
import sys
import pkgutil

pkg_dir = os.path.dirname(__file__)
for (module_loader, name, ispkg) in pkgutil.iter_modules([pkg_dir]):
    pkgutil.importlib.import_module('.' + name, __package__)

# -----------------------------------------------------------------------------
# Module interface
# -----------------------------------------------------------------------------

# Usage:
#
# from scapula.filters import filters
# registers = filters["filter_name"].filter_exclusive(registers)
# registers = filters["filter_name"].filter_inclusive(registers)

filters = {
    "register_has_res0": register_has_res0.RegisterHasRES0Filter(),
    "register_has_res1": register_has_res1.RegisterHasRES1Filter(),
    "register_has_imp_def": register_has_imp_def.RegisterHasImpDefFilter()
}
