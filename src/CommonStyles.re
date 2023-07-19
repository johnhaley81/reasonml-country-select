module Colors = {
  open Css;

  module Light = {
    module Background = {
      let box = hex("FFFFFF");
      let selected = hex("FFDBB3");
    };

    module Border = {
      let controlAlpha = hex("000000");
      let lineAlpha = hex("000000");
    };

    module Text = {
      let primary = hex("333333");
      let secondary = hex("000000");
      let tertiary = hex("000000");
    };
  };
};
