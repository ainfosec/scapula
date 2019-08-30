import shoulder

class OnlyImpDefTransform(shoulder.transform.abstract_transform.AbstractTransform):
    @property
    def description(self):
        d = "removing non IMPLEMENTATION_DEFINED fields"
        return d

    def do_transform(self, reg):
        for fs in reg.fieldsets:
            fs_len = len(fs.fields)
            fs.fields = [field for field in fs.fields if "IMPLEMENTATION_DEFINED" == field.name]

            count = fs_len - len(fs.fields)
            if count:
                shoulder.logger.debug("Removed {count} field{s} from {reg}".format(
                    count = count,
                    reg = reg.name,
                    s = "" if count == 1 else "s"
                ))

        return reg
