module ClassNames = {
  type state;

  // This is a partial list of keys. A more complete list can be found at:
  // https://react-select.com/styles#inner-components
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    container: state => string,
    [@bs.optional]
    control: state => string,
    [@bs.optional]
    menu: state => string,
    [@bs.optional]
    placeholder: state => string,
  };

  // [@bs.deriving abstract] creates a function that constructs the type and defaults it to be the
  // same name as the type. It also creates a getter function for each field on the type. With that
  // being said, I often like to wrap these in modules so as to not pollute the namespace where
  // the type is created. I also like to rename the constructor function to `make` so I can provide
  // a more consistent and standard interface.
  let make = t;
};

module Components = {
  type indicatorsContainerProps = {
    innerRef: Js.Nullable.t(ReactDOM.domRef),
    innerProps: Js.t({.}),
  };

  type optionProps('a) = {
    data: 'a,
    innerProps: Js.t({.}),
    isFocused: bool,
    isSelected: bool,
  };

  [@bs.deriving abstract]
  type t('a) = {
    [@bs.as "IndicatorsContainer"] [@bs.optional]
    indicatorsContainer: indicatorsContainerProps => React.element,
    [@bs.as "Option"] [@bs.optional]
    option: optionProps('a) => React.element,
  };

  let make = t;
};

module Select = {
  [@bs.module "react-select"] [@react.component]
  external make:
    (
      ~autoFocus: bool=?,
      ~className: string=?,
      ~classNames: ClassNames.t=?,
      ~closeMenuOnSelect: bool=?,
      ~components: Components.t('a)=?,
      ~defaultValue: 'a=?,
      ~isLoading: bool=?,
      ~isOptionSelected: 'a => bool=?,
      ~menuIsOpen: bool=?,
      ~onChange: Js.Nullable.t('a) => unit=?,
      ~onKeyDown: ReactEvent.Keyboard.t => unit=?,
      ~options: array('a),
      ~ref: ReactDOM.Ref.t=?,
      ~placeholder: string=?,
      ~unstyled: bool=?
    ) =>
    React.element =
    "default";

  let makeProps = (~onChange=?, ~onKeyDown=?) =>
    makeProps(
      ~onChange=?
        onChange
        |> Option.map(onChange =>
             Js.Nullable.toOption >> onChange >> IOUtils.unsafeRunHandledAsync
           ),
      ~onKeyDown=?
        onKeyDown
        |> Option.map(onKeyDown => onKeyDown >> IOUtils.unsafeRunHandledAsync),
    );
};
