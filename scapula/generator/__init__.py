import os
import pkgutil
from collections import OrderedDict

pkg_dir = os.path.dirname(__file__)
for (module_loader, name, ispkg) in pkgutil.iter_modules([pkg_dir]):
    pkgutil.importlib.import_module('.' + name, __package__)

generators = OrderedDict()
generators["read_res0_generator"] = read_res0_generator.ReadRes0Generator()
generators["read_res1_generator"] = read_res1_generator.ReadRes1Generator()
generators["read_imp_def_generator"] = read_imp_def_generator.ReadImpDefGenerator()
