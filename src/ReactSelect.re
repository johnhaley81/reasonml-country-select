// module CommonProps = {
//   // This is pulled from:
//   // https://github.com/JedWatson/react-select/blob/011a9d9bb38dfa65ab493927c84f40814f907226/packages/react-select/src/types.ts#L72
//   [@bs.deriving abstract]
//   type t = {
//     [@bs.as "DropdownIndicator"]
//     dropdownIndicator: unit,
//   };
// };

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

  let make = t;
};

module Components = {
  type indicatorsContainerProps = {
    innerRef: Js.Nullable.t(ReactDOM.domRef),
    innerProps: Js.t({.}),
  };

  [@bs.deriving abstract]
  type t = {
    [@bs.as "IndicatorsContainer"] [@bs.optional]
    indicatorsContainer: indicatorsContainerProps => React.element,
  };

  let make = t;
};

module Select = {
  [@bs.module "react-select"] [@react.component]
  external make:
    (
      ~className: string=?,
      ~classNames: ClassNames.t=?,
      ~components: Components.t=?,
      ~defaultValue: 'a=?,
      ~formatOptionLabel: 'a => React.element=?,
      ~options: array('a),
      ~placeholder: string=?,
      ~unstyled: bool=?
    ) =>
    React.element =
    "default";
};
