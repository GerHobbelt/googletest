from conans import ConanFile


class GoogletestConan(ConanFile):
    name = "googletest"
    version = "1.13.x"
    url = "https://github.com/Esri/googletest/tree/runtimecore"
    license = "https://github.com/Esri/googletest/blob/runtimecore/LICENSE"
    description = "Googletest - Google Testing and Mocking Framework."

    # RTC specific triple
    settings = "platform_architecture_target"

    def package(self):
        base = self.source_folder + "/"
        relative = "3rdparty/googletest/"

        # headers
        self.copy("*.h*", src=base + "googlemock/include", dst=relative + "googlemock/include", excludes="*.pump")
        self.copy("*.h*", src=base + "googletest/include", dst=relative + "googletest/include")

        # libraries
        output = "output/" + str(self.settings.platform_architecture_target) + "/staticlib"
        self.copy("*" + self.name + "*", src=base + "../../" + output, dst=output)
