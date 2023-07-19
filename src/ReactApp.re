open ReactUtils;

module App = {
  [@react.component]
  let make = () =>
    <>
      {["Hello " ++ World.name ++ "!", "This is React! With Relude!!"]
       |> List.map(greeting => <h1> <S> greeting </S> </h1>)
       |> List.toArray
       |> React.array}
      <FlagIcons
        countryCode={FlagIcons.CountryCode.make("gr") |> Option.getOrThrow}
        // displayFormat=Square
      />
    </>;
};

ReactDOM.querySelector("#root")
|> Option.foldLazy(
     () =>
       Js.Console.error(
         "Failed to start React: couldn't find the #root element",
       ),
     ReactDOM.render(<App />),
   );
