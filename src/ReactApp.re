module React18Dom = {
  // This is to suppress this warning:
  // https://react.dev/blog/2022/03/08/react-18-upgrade-guide#updates-to-client-rendering-apis
  type root;
  [@bs.module "react-dom/client"]
  external createRoot: Dom.element => root = "createRoot";

  // This is how you are supposed to mount react into the DOM in React 18
  [@bs.send] external render: (root, React.element) => unit = "render";
  let render = flip(render);
};

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
    let (country, setCountry) = React.useState(() => None);

    <div className=Styles.wrapper>
      <CountrySelect country onChange={value => setCountry(const(value))} />
    </div>;
  };
};

ReactDOM.querySelector("#root")
|> Option.map(React18Dom.createRoot)
|> Option.foldLazy(
     () =>
       Js.Console.error(
         "Failed to start React: couldn't find the #root element",
       ),
     React18Dom.render(<App />),
   );
