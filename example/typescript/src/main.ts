
namespace flutter {
    function homePage() {
        let pressCount = 0
        return StatefulBuilder({
            builder(context, setState): Widget {
                let widget: Widget

                widget = Scaffold({
                    appBar: AppBar({
                        title: Text("Lua Flutter Demo Home Page"),
                        backgroundColor: Theme.of(context).colorScheme.inversePrimary
                    }),
                    
                    body: Center ({
                        child: Column({
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                                Text('You have pushed the button this many times:'),
                                Text(`${pressCount}`, {style: Theme.of(context).textTheme.headlineMedium}),
                            ]
                        })
                    }),

                    floatingActionButton: Column({
                        mainAxisSize: MainAxisSize.min,
                        children: [
                            FloatingActionButton({
                                onPressed() {
                                    pressCount += 1
                                    setState()
                                },
                                tooltip: 'Increment',
                                child: Icon(Icons.add),
                            }),

                            SizedBox({height: 20}),

                            FloatingActionButton({
                                onPressed() {
                                    pressCount -= 1
                                    setState()
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
    }

    export function main() {
        let widget: Widget 
        widget = Builder({
            builder(context) {
                return MaterialApp({
                    title: "Flutter Demo",
                    theme: ThemeData({
                        colorScheme: ColorScheme.fromSeed({seedColor: Colors.deepPurple}), 
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
