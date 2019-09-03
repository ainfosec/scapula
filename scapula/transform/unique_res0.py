import shoulder

class UniqueRES0Transform(shoulder.transform.abstract_transform.AbstractTransform):
    @property
    def description(self):
        d = "giving unique names per-register to reserved 0 (RES0) fields"
        return d

    def do_transform(self, reg):
        for fs in reg.fieldsets:
            res0_count = 0
            for f in fs.fields:
                if f.name == "0":
                    f.name = "0_" + str(res0_count)
                    res0_count += 1

        return reg
