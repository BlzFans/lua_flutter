
local function homePage()
    local pressCount = 0
    local widget
    widget = LStatefulWidget {
        stateCreator = function()
            local state
            state = LState{
                builder = function(context)
                    local widget = Scaffold {
                            appBar = AppBar{
                                title = Text("Flutter Demo Home Page"),
                                backgroundColor = Theme.of(context).colorScheme.inversePrimary
                            },
                            body = Center{
                                child = Column{
                                    mainAxisAlignment = MainAxisAlignment.center,
                                    children = {
                                        Text('You have pushed the button this many times:'),
                                        Text(tostring(pressCount), {style = Theme.of(context).textTheme.headlineMedium}),
                                    },
                                },
                            },

                            floatingActionButton = Column {
                                mainAxisSize = MainAxisSize.min,
                                children = {
                                    FloatingActionButton{
                                        onPressed = function()
                                            pressCount = pressCount + 1
                                            state:markNeedsBuild()
                                        end,
                                        tooltip = 'Increment',
                                        child = Icon(Icons.add),
                                    },
                                    SizedBox{height = 20},
                                    FloatingActionButton{
                                        onPressed = function()
                                            pressCount = pressCount - 1
                                            state:markNeedsBuild()
                                        end,
                                        tooltip = 'Decrement',
                                        child = Icon(Icons.remove),
                                    },
                                }
                            }
                        }

                    return widget
                end
            }
            
            return state
        end
    }
    
    return widget
end
                 
local function main()
    local widget 
    widget = LStatelessWidget {
        builder = function (context)
            return MaterialApp {
                title = "Flutter Demo",
                theme = ThemeData{
                    colorScheme = ColorScheme.fromSeed{seedColor = Colors.deepPurple}, 
                    useMaterial3 = true,
                },
                debugShowCheckedModeBanner = false,
                navigatorKey = NavigationService.navigatorKey,
                home = homePage()
            }
        end
    }
    
    runApp(widget)
end

main()
