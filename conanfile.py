from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout


class SetUp(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    options = {"sanitizer": ["none", "asan", "tsan", "msan", "ubsan"]}
    default_options = {"sanitizer": "none"}

    def configure(self):
        flags = []
        
        if self.settings.build_type == "Debug":
            # Warning + Debug flags
            flags.extend([
                "-Wall", "-Wextra", "-Wpedantic", "-Wshadow", "-Wnon-virtual-dtor",
                "-Wcast-align", "-Wunused", "-Woverloaded-virtual", "-Wdouble-promotion",
                "-Wconversion", "-Wmisleading-indentation"
            ])

            flags.extend([ "-O0", "-g", "-fno-omit-frame-pointer" ])
            
            sanitizer_map = {
                "asan": ["-fsanitize=address", "-fsanitize=leak"],
                "tsan": ["-fsanitize=thread"], 
                "msan": ["-fsanitize=memory"],
                "ubsan": ["-fsanitize=undefined"]
            }
            
            if self.options.sanitizer in sanitizer_map:
                sanitizer_flags = sanitizer_map[self.options.sanitizer]
                flags.extend(sanitizer_flags)
                self.conf.define("tools.build:exelinkflags", sanitizer_flags)
                self.conf.define("tools.build:sharedlinkflags", sanitizer_flags)
                
        elif self.settings.build_type == "Release":
            flags.extend(["-O3", "-DNDEBUG", "-march=native"])
        
        self.conf.define("tools.build:cflags", flags)
        self.conf.define("tools.build:cxxflags", flags)

    def generate(self):
        cmake = CMakeToolchain(self)
        cmake.generator = "Ninja"
        if self.settings.compiler.cppstd:
            cmake.variables["CMAKE_CXX_STANDARD"] = str(self.settings.compiler.cppstd)
            cmake.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"
        cmake.generate()

        CMakeDeps(self).generate()

    def layout(self):
        cmake_layout(self)