module Select = {
  [@bs.module "react-select"] [@react.component]
  external make:
    (
      ~defaultValue: 'a=?,
      ~formatOptionLabel: 'a => React.element=?,
      ~options: array('a)
    ) =>
    React.element =
    "default";
};
