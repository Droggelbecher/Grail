
package.cpath = package.cpath..";./?.dll;./?.so;../lib/?.so;../lib/vc_dll/?.dll;../lib/bcc_dll/?.dll;../lib/mingw_dll/?.dll;/usr/lib/lua/5.1/?.so"

local function searcher(name)
  local function loader()
    print("    loading package: "..name)
    local path, _ = '/' .. string.gsub(name, '\\.', '/')
	local p = path .. ".lua"
	print("---------------------")
	print("p=" .. p)
	if GAME:getResourceManager():exists(p) then
		local r = doresource(p)
		if type(package.loaded[name]) ~= "table" then
		  print("/!\\ Package \"" .. name .. "\" has a missing or wrong module declaration, loading it will probably fail!")
		end
	end
	print("---------------------")
  end

  return loader
end

local old_loaders = package.loaders
table.insert(old_loaders, searcher)
table.foreach(old_loaders, print)
package.loaders = old_loaders
