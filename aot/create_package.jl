using PackageCompiler
create_sysimage([:TestCall, :SpecialFunctions], replace_default=true, precompile_execution_file="precompile_testcall.jl")