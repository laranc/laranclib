return {
	name = "laranclib",
	includes = { "include" },
	build = function(dir, rules)
		local lib = StaticLib:Create(StaticLibOptions("laranclib", CompilerFlags("DEFAULT", "OFF", "DEFAULT", "C23")))
		lib:AddFiles(dir .. "/src/*.c")
		lib:AddIncludePaths(dir .. "/include")
		lib:Install()
		return lib
	end,
}
