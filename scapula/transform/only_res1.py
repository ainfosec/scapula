import shoulder

class OnlyRes1Transform(shoulder.transform.abstract_transform.AbstractTransform):
    @property
    def description(self):
        d = "removing non reserved 1 (RES1) fields"
        return d

    def do_transform(self, reg):
        for fs in reg.fieldsets:
            fs_len = len(fs.fields)
            fs.fields = [field for field in fs.fields if "1" == field.name]

            count = fs_len - len(fs.fields)
            if count:
                shoulder.logger.debug("Removed {count} field{s} from {reg}".format(
                    count = count,
                    reg = reg.name,
                    s = "" if count == 1 else "s"
                ))

        return reg
