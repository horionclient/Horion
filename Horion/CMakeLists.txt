file(GLOB_RECURSE TargetFiles
        "*.h"
        "*.cpp")

target_sources(Horion PUBLIC ${TargetFiles})