open ReactUtils;

module Menu = {
  module Styles = {
    open Css;
    let wrapper = style([position(absolute), zIndex(2)]);
  };

  [@react.component]
  let make = (~children) => <div className=Styles.wrapper> children </div>;
};

module Blanket = {
  module Styles = {
    open Css;

    let wrapper =
      style([
        bottom(zero),
        left(zero),
        position(fixed),
        right(zero),
        top(zero),
        zIndex(1),
      ]);
  };

  [@react.component]
  let make = (~onClose) =>
    <div
      className=Styles.wrapper
      onClick={const(onClose) >> IOUtils.unsafeRunHandledAsync}
    />;
};

module Dropdown = {
  module Styles = {
    open Css;

    let wrapper = style([position(relative)]);
  };

  [@react.component]
  let make = (~children, ~isOpen, ~target, ~onClose) =>
    <div className=Styles.wrapper>
      target
      {isOpen ? <> <Menu> children </Menu> <Blanket onClose /> </> : <Null />}
    </div>;
};

[@react.component]
let make = () => {
  let (isOpen, setIsOpen) = ReactUtils.useState(() => false);

  <Dropdown
    isOpen
    onClose={IO.suspendIO(() => setIsOpen(false))}
    target={
      <button
        onClick={const(true) >> setIsOpen >> IOUtils.unsafeRunHandledAsync}>
        <S> "Test" </S>
        <Icons.TriangleNeutral />
      </button>
    }>
    <div> <S> "YAY" </S> </div>
  </Dropdown>;
};
