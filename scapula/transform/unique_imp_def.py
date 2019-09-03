import shoulder

class UniqueImpDefTransform(shoulder.transform.abstract_transform.AbstractTransform):
    @property
    def description(self):
        d = "giving unique names per-register to IMPLEMENTATION_DEFINED fields"
        return d

    def do_transform(self, reg):
        for fs in reg.fieldsets:
            reg_count = 0
            for f in fs.fields:
                if f.name == "IMPLEMENTATION_DEFINED":
                    f.name = "IMPLEMENTATION_DEFINED_" + str(reg_count)
                    reg_count += 1

        return reg
