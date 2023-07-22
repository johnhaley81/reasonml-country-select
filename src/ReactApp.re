module App = {
  module Styles = {
    open Css;

    let wrapper =
      style([
        alignItems(flexStart),
        backgroundColor(CommonStyles.Colors.Light.Background.box),
        display(inlineFlex),
        padding2(~v=rem(8.0), ~h=rem(16.5626)),
      ]);
  };

  [@react.component]
  let make = () => {
    let country =
      CountrySelect.Country.make(
        ~amount=10.0,
        ~countryCode="us",
        ~label="United States",
      );

    <div className=Styles.wrapper> <CountrySelect country /> </div>;
  };
};

ReactDOM.querySelector("#root")
|> Option.foldLazy(
     () =>
       Js.Console.error(
         "Failed to start React: couldn't find the #root element",
       ),
     ReactDOM.render(<App />),
   );
