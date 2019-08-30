import shoulder

class RegisterHasImpDefFilter(shoulder.filter.abstract_filter.AbstractFilter):
    @property
    def description(self):
        return "registers that contain IMPLEMENTATION_DEFINED fields"

    def do_filter(self, reg):
        for fs in reg.fieldsets:
            for f in fs.fields:
                if f.name == "IMPLEMENTATION_DEFINED":
                    return False

        return True
