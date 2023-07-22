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
  let make = (~children, ~className=?, ~isOpen, ~target, ~onClose) =>
    <div className={className |> ReactUtils.extendBaseStyle(Styles.wrapper)}>
      target
      {isOpen ? <> <Menu> children </Menu> <Blanket onClose /> </> : <Null />}
    </div>;
};

module Country = {
  type t = {
    amount: float,
    countryCode: FlagIcons.CountryCode.t,
    label: string,
  };

  let make = (~amount, ~countryCode as maybeCountryCode, ~label) =>
    maybeCountryCode
    |> FlagIcons.CountryCode.make
    |> Option.map(countryCode => {amount, countryCode, label});
};

module Button = {
  module Styles = {
    open Css;
    let wrapper =
      style([
        backgroundColor(transparent),
        display(flexBox),
        height(rem(1.625)),
        padding2(~v=rem(0.25), ~h=rem(0.625)),
        alignItems(center),
        gap(rem(0.375)),
        borderRadius(rem(0.1875)),
        border(
          px(1),
          solid,
          CommonStyles.Colors.Light.Border.controlAlpha(0.20),
        ),
      ])
      |> Css.merge2(CommonStyles.Text.defaultStyles);

    let contents = style([display(flexBox), alignItems(center)]);

    let flagIcon = style([marginRight(rem(0.5))]);

    let flagIconPlaceholder =
      style([
        height(rem(0.875)),
        width(rem(0.875)),
        paddingRight(rem(0.5)),
      ]);

    let triangle = style([marginLeft(rem(0.38))]);
  };

  [@react.component]
  let make = (~className=?, ~country, ~onClick) =>
    <button
      className={className |> ReactUtils.extendBaseStyle(Styles.wrapper)}
      onClick={const(onClick) >> IOUtils.unsafeRunHandledAsync}>
      <span className=Styles.contents>
        {country
         |> Option.fold(
              <>
                <div className=Styles.flagIconPlaceholder />
                <div> <S> "None" </S> </div>
              </>,
              ({amount: _, countryCode, label}: Country.t) =>
              <>
                <FlagIcons className=Styles.flagIcon countryCode />
                <div> <S> label </S> </div>
              </>
            )}
        <Icons.TriangleNeutral className=Styles.triangle />
      </span>
    </button>;
};

module Styles = {
  open Css;

  let wrapper = style([]);
};
let formatOptionLabel = (Country.{countryCode: _, label, amount}) =>
  <div> <div> <S> label </S> </div> <div> <F> amount </F> </div> </div>;

[@react.component]
let make = (~className=?, ~country) => {
  let (isOpen, setIsOpen) = ReactUtils.useState(() => false);

  let options =
    Country.(
      [|
        make(~amount=10.0, ~countryCode="us", ~label="United States"),
        make(~amount=200.0, ~countryCode="af", ~label="Afghanistan"),
        make(~amount=5000.0, ~countryCode="ca", ~label="Canada"),
      |]
    )
    |> Array.catOption;

  let country =
    options
    |> Array.find((Country.{amount: _, countryCode, label: _}) =>
         countryCode
         |> FlagIcons.CountryCode.toString
         |> Option.pure
         |> Option.eqBy(String.eq, country)
       );

  <Dropdown
    className={className |> ReactUtils.extendBaseStyle(Styles.wrapper)}
    isOpen
    onClose={IO.suspendIO(() => setIsOpen(false))}
    target={<Button country onClick={setIsOpen(true)} />}>
    <ReactSelect.Select formatOptionLabel options />
  </Dropdown>;
};
