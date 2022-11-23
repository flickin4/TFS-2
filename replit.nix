{ pkgs }: {
	deps = [
		pkgs.unixtools.xxd
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}