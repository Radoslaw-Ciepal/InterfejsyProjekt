{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  outputs =
    { nixpkgs, ... }:
    let
      system = "aarch64-darwin";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          qt6.qtbase
        ];
        packages = with pkgs; [
          clang-tools
          cmake
          cmake-language-server
          ninja
        ];
      };
    };
}
