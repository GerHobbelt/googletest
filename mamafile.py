import mama

##
# Explore Mama docs at https://github.com/RedFox20/Mama
#
class googletest(mama.BuildTarget):

    def dependencies(self):
        pass

    def configure(self):
        self.enable_cxx20()

    def package(self):
        self.export_includes(['include'], build_dir=True)
        # export gmock and gtest main libs only 
        self.export_libs('lib/', [
            'gmock.lib', 'gmock.a',
            'gtest.lib', 'gtest.a'
        ])

