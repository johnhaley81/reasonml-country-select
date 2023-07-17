module App = {
  [@react.component]
  let make = () =>
    ["Hello " ++ World.name ++ "!", "This is React! With Relude!"]
    |> Relude.List.map(greeting => <h1> greeting->React.string </h1>)
    |> Relude.List.toArray
    |> React.array;
};

ReactDOM.querySelector("#root")
|> Relude.Option.foldLazy(
     () =>
       Js.Console.error(
         "Failed to start React: couldn't find the #root element",
       ),
     ReactDOM.render(<App />),
   );
