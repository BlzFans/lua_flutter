# lua_flutter

Create flutter app using lua and typescript

## Lua
```lua title=main.lua
--main.lua
local widget = Center {
  child = Text(
      "flutter_main", {
          style = TextStyle {
              fontWeight = FontWeight.bold,
              color = Colors.blue,
          },
          textDirection = TextDirection.ltr,
      }
  )
}

runApp(widget)
```

## Typescript
```typescript title=main.ts
//main.ts
namespace flutter {
    function homePage() {
        let pressCount = 0
        return LStatefulWidget({
            stateCreator() {
                let state = LState({
                    builder(context): Widget {
                        let widget: Widget
                        let theme = Theme.of(context)
                        let colorScheme = theme.colorScheme
                        let textTheme = theme.textTheme

                        widget = Scaffold({
                            appBar: AppBar({
                                title: Text("Typescript Flutter Demo Home Page"),
                                backgroundColor: colorScheme.inversePrimary
                            }),
                            
                            body: Center ({
                                child: Column({
                                    mainAxisAlignment: MainAxisAlignment.center,
                                    children: [
                                        Text('You have pushed the button this many times:'),
                                        Text(`${pressCount}`, {
                                            style: textTheme.headlineMedium
                                        }),
                                    ]
                                })
                            }),

                            floatingActionButton: Column({
                                mainAxisSize: MainAxisSize.min,
                                children: [
                                    FloatingActionButton({
                                        onPressed() {
                                            pressCount += 1
                                            state.markNeedsBuild()
                                        },
                                        tooltip: 'Increment',
                                        child: Icon(Icons.add),
                                    }),

                                    SizedBox({height: 20}),

                                    FloatingActionButton({
                                        onPressed() {
                                            pressCount -= 1
                                            state.markNeedsBuild()
                                        },
                                        tooltip: 'Decrement',
                                        child: Icon(Icons.remove),
                                    }),
                                ]
                            })
                        })

                        return widget
                    }
                })

                return state
            }
        })
    }

    export function main() {
        let widget: Widget 
        widget = LStatelessWidget({
            builder(context) {
                return MaterialApp({
                    title: "Flutter Demo",
                    theme: ThemeData({
                        colorSchemeSeed: Colors.deepPurple, 
                        useMaterial3: true,
                    }),
                    debugShowCheckedModeBanner: false,
                    navigatorKey: NavigationService.navigatorKey,
                    home: homePage()
                })
            }
        })
        
        runApp(widget)
    }
}

flutter.main()
```
