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
  let make = (~children, ~className=?, ~isOpen, ~target, ~setIsOpen) =>
    <div
      className={className |> ReactUtils.extendBaseStyle(Styles.wrapper)}
      onKeyDown={
        DomUtils.getKeyFromEvent
        >> Option.map(Tuple.first)
        >> Option.foldLazy(
             IO.pure,
             fun
             | (EnterKey: DomUtils.key)
             | ArrowDown => true |> setIsOpen
             | EscapeKey
             | ArrowUp => IO.pure(),
           )
        >> IOUtils.unsafeRunHandledAsync
      }
      tabIndex=0>
      target
      {isOpen
         ? <>
             <Menu> children </Menu>
             <Blanket onClose={false |> setIsOpen} />
           </>
         : <Null />}
    </div>;
};

module Country = {
  type t = {
    countryCode: FlagIcons.CountryCode.t,
    label: string,
    value: float,
  };

  let make = (~countryCode as maybeCountryCode, ~label, ~value) =>
    maybeCountryCode
    |> FlagIcons.CountryCode.make
    |> Option.map(countryCode => {countryCode, label, value});

  let url = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";

  let fetchCountries =
    url
    |> Fetch.fetch
    |> IO.flatMap(Fetch.json)
    |> IO.map(
         Js.Json.decodeArray
         >> Option.getOrElse([||])
         >> Array.mapOption(Js.Json.decodeObject)
         >> Array.mapOption(dict =>
              Option.Infix.(
                (
                  (countryCode, label) =>
                    make(~countryCode, ~label, ~value=0.0)
                )
                <$> (
                  Js.Dict.get(dict, "value")
                  |> Option.flatMap(Js.Json.decodeString)
                )
                <*> (
                  Js.Dict.get(dict, "label")
                  |> Option.flatMap(Js.Json.decodeString)
                )
              )
            )
         >> Array.catOption,
       );
};

module EmptyFlagIcon = {
  module Styles = {
    open Css;

    let flagIconPlaceholder =
      style([
        height(rem(0.875)),
        width(rem(0.875)),
        paddingRight(rem(0.5)),
      ]);
  };

  [@react.component]
  let make = () => <div className=Styles.flagIconPlaceholder />;
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

    let triangle = style([marginLeft(rem(0.38))]);
  };

  [@react.component]
  let make = (~className=?, ~countries, ~onClick, ~selectedCountry) =>
    <button
      className={className |> ReactUtils.extendBaseStyle(Styles.wrapper)}
      onClick={const(onClick) >> IOUtils.unsafeRunHandledAsync}>
      <span className=Styles.contents>
        {countries
         |> AsyncResult.foldByValue(
              <> <EmptyFlagIcon /> <div> <S> "Loading..." </S> </div> </>,
              Array.find((Country.{countryCode, label: _, value: _}) =>
                countryCode
                |> FlagIcons.CountryCode.toString
                |> Option.pure
                |> Option.eqBy(String.eq, selectedCountry)
              )
              >> Option.fold(
                   <>
                     <EmptyFlagIcon />
                     <div> <S> "Select a Country" </S> </div>
                   </>,
                   ({countryCode, label, value: _}: Country.t) =>
                   <>
                     <FlagIcons className=Styles.flagIcon countryCode />
                     <div> <S> label </S> </div>
                   </>
                 ),
              _e =>
              <>
                <EmptyFlagIcon />
                <div> <S> "Error in loading countries" </S> </div>
              </>
            )}
        <Icons.TriangleNeutral className=Styles.triangle />
      </span>
    </button>;
};

module MenuOption = {
  module Styles = {
    open Css;

    let container = (~isFocused, ~isSelected) =>
      style([
        alignItems(center),
        alignSelf(stretch),
        backgroundColor(
          isSelected
            ? CommonStyles.Colors.Light.Background.selected(1.0)
            : isFocused
                ? CommonStyles.Colors.Light.Background.selected(0.25)
                : transparent,
        ),
        display(flexBox),
        gap(rem(0.5)),
        justifyContent(spaceBetween),
        padding4(
          ~top=rem(0.25),
          ~right=rem(0.375),
          ~bottom=rem(0.25),
          ~left=rem(0.62),
        ),
      ]);

    let flagAndLabelWrapper = style([display(flexBox)]);

    let flagIcon = style([marginRight(rem(0.5))]);

    let amount =
      style([
        alignItems(center),
        borderRadius(rem(1.25)),
        display(flexBox),
        padding2(~v=rem(0.125), ~h=rem(0.25)),
        gap(rem(0.5)),
        color(CommonStyles.Colors.Light.Text.secondaryWithAlpha(0.52)),
        textAlign(center),
        fontFamily(`custom("Arial")),
        fontSize(rem(0.75)),
        fontStyle(normal),
        fontWeight(`num(400)),
        lineHeight(rem(0.875)),
      ]);
  };

  [@react.component]
  let make =
      (
        ~country,
        ~optionProps as
          {
            data: {countryCode, label, value},
            innerProps,
            isFocused,
            // So the `isSelected` prop here isn't acting the way I anticipated, so instead of relying on
            // the internal state of react-select we going to rely on the passed in `country` prop
            isSelected: _,
          }:
            ReactSelect.Components.optionProps(Country.t),
      ) => {
    // Loading the flag makes a network request and if that request isn't already cached _and_ we're making a lot of
    // them, then it could take quite a bit of time for all of the flags to load. This stops all of the flags from
    // being loaded at once and allows only the flags that are visible to be loaded.
    let (isInViewport, setIsInViewport) = ReactUtils.useState(() => false);
    let (domRef, setDomRef) = ReactUtils.useDomRef();

    domRef
    |> ReactUtils.useEffect1(
         ~onUnmount=
           Result.fold(IO.pure, DomUtils.IntersectionObserver.disconnect),
         IO.flatMap(maybeEl =>
           DomUtils.IntersectionObserver.make(
             Array.head
             >> Option.foldLazy(
                  IO.pure,
                  DomUtils.IntersectionObserver.IntersectionObserverEntry.isIntersecting
                  >> setIsInViewport,
                ),
           )
           |> IO.pure
           |> IO.flatMap(observer =>
                maybeEl
                |> Option.fold(
                     observer |> IO.pure,
                     DomUtils.IntersectionObserver.observe(_, observer),
                   )
              )
         ),
       );

    (isFocused, domRef)
    |> ReactUtils.useEffect2(
         DomUtils.scrollRefIntoViewOnCondition(
           ~scrollOptions=
             DomUtils.ScrollOptions.make(
               ~block=`nearest,
               ~inline=`nearest,
               ~scrollMode=`ifNeeded,
               (),
             ),
         ),
       );

    <Spread props=innerProps>
      <div
        className={Styles.container(
          ~isFocused,
          ~isSelected=
            {countryCode
             |> FlagIcons.CountryCode.toString
             |> Option.pure
             |> Option.eqBy((===), country)},
        )}
        ref=setDomRef>
        <div className=Styles.flagAndLabelWrapper>
          {isInViewport
             ? <FlagIcons className=Styles.flagIcon countryCode />
             : <EmptyFlagIcon />}
          <div> <S> label </S> </div>
        </div>
        <div className=Styles.amount> <F> value </F> </div>
      </div>
    </Spread>;
  };
};

module Styles = {
  open Css;

  let container =
    const(
      style([
        alignItems(flexStart),
        backgroundColor(CommonStyles.Colors.Light.Background.box),
        borderRadius(rem(0.125)),
        boxShadows([
          Shadow.box(
            ~x=zero,
            ~y=px(3),
            ~blur=px(18),
            ~spread=zero,
            rgba(0, 0, 0, `num(0.15)),
          ),
          Shadow.box(
            ~x=zero,
            ~y=zero,
            ~blur=zero,
            ~spread=px(1),
            rgba(0, 0, 0, `num(0.08)),
          ),
        ]),
        display(flexBox),
        flexDirection(column),
        marginTop(rem(0.19)),
        maxHeight(rem(25.0)),
        padding2(~v=rem(0.25), ~h=zero),
        width(rem(14.375)),
      ])
      |> Css.merge2(CommonStyles.Text.defaultStyles),
    );

  let control =
    const(
      style([
        alignItems(center),
        alignSelf(stretch),
        display(flexBox),
        flexDirection(rowReverse),
        flexShrink(0.0),
        gap(rem(0.5)),
        height(rem(1.625)),
        important(minHeight(inherit_)),
        padding2(~v=zero, ~h=rem(0.625)),
      ]),
    );

  let menu =
    const(
      style([
        borderTop(
          rem(0.0625),
          solid,
          CommonStyles.Colors.Light.Border.lineAlpha(0.08),
        ),
        marginTop(rem(0.25)),
        paddingTop(rem(0.25)),
        important(position(inherit_)),
      ]),
    );

  let placeholder =
    const(
      style([
        color(CommonStyles.Colors.Light.Text.tertiaryWithAlpha(0.32)),
        overflow(hidden),
        textOverflow(ellipsis),
        whiteSpace(nowrap),
      ]),
    );
};

[@react.component]
let make = (~className=?, ~country, ~onChange) => {
  let (isOpen, setIsOpen) = ReactUtils.useState(() => false);
  let (options, setOptions) = ReactUtils.useState(() => AsyncResult.init);

  ReactUtils.useEffect0(
    setOptions(AsyncResult.loading)
    |> IO.flatMap(() => Country.fetchCountries)
    |> IO.flatMap(AsyncResult.completeOk >> setOptions),
  );

  <Dropdown
    ?className
    isOpen
    setIsOpen
    target={
      <Button
        countries=options
        onClick={true |> setIsOpen}
        selectedCountry=country
      />
    }>
    <ReactSelect.Select
      autoFocus=true
      classNames={ReactSelect.ClassNames.make(
        ~container=Styles.container,
        ~control=Styles.control,
        ~menu=Styles.menu,
        ~placeholder=Styles.placeholder,
        (),
      )}
      components={ReactSelect.Components.make(
        ~indicatorsContainer=
          ({innerRef, innerProps}) =>
            <Spread props=innerProps>
              <div ref=?{innerRef |> Js.Nullable.toOption}>
                <Icons.Search />
              </div>
            </Spread>,
        ~option=optionProps => <MenuOption country optionProps />,
        (),
      )}
      isLoading={options |> AsyncResult.isLoading}
      onChange={
        Option.map(({countryCode, label: _, value: _}: Country.t) =>
          countryCode |> FlagIcons.CountryCode.toString
        )
        >> IO.pure
        >> IO.map(onChange)
        >> IO.flatMap(() => false |> setIsOpen)
      }
      onKeyDown={e => {
        // The dropdown will also process the keydown and might possibly stop the
        // menu from closing.
        e |> ReactEvent.Keyboard.stopPropagation;
        e
        |> DomUtils.getKeyFromEvent
        |> Option.map(Tuple.first)
        |> Option.foldLazy(
             IO.pure,
             fun
             | (EscapeKey: DomUtils.key) => false |> setIsOpen
             | EnterKey
             | ArrowUp
             | ArrowDown => IO.pure(),
           );
      }}
      options={options |> AsyncResult.getOk |> Option.getOrElse([||])}
      menuIsOpen=isOpen
      placeholder="Search"
      unstyled=true
    />
  </Dropdown>;
};
