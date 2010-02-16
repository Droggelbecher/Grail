
local function searcher(name)
  local function loader()
    print("    loading package: "..name)
    path, _ = '/' .. string.gsub(name, '\\.', '/')
    r = doresource(path .. ".lua")
    if type(package.loaded[name]) ~= "table" then
      print("/!\\ Package \"" .. name .. "\" has a missing or wrong module declaration, loading it will probably fail!")
    end
  end

  return loader
end

package.loaders = { searcher }

