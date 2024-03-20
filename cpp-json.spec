Name:           tokox-cpp-json
Version:        1.2
Release:        2%{?dist}
Summary:        C++ JSON object import/export library

License:        MIT
URL:            https://github.com/tokox/cpp-json
BugURL:         https://github.com/tokox/cpp-json/issues
Source0:        https://github.com/tokox/cpp-json/archive/v%{version}.tar.gz

BuildArch:      noarch

%description
This is a JSON object import/export library in C++. It contains class to represent JSON Object and functions to read it from stream and write it to stream. That is basically it. For more information look at README.
Install from copr:
```
dnf copr enable tokox/cpp-json
dnf install tokox-cpp-json
```
Library should install in `/usr/include/tokox/cpp-json/` and docs should install in `/usr/share/doc/tokox/cpp-json/`

Bugs and issues report at github: tokox/cpp-json

%prep
%setup -n cpp-json-%{version}

%install
mkdir -p %{buildroot}/usr/include/tokox/cpp-json
cp -a json.hpp %{buildroot}/usr/include/tokox/cpp-json
mkdir -p %{buildroot}/usr/share/doc/tokox/cpp-json
cp -a LICENSE.md README.md %{buildroot}/usr/share/doc/tokox/cpp-json

%files
          /usr/include/tokox/cpp-json/json.hpp
%license  /usr/share/doc/tokox/cpp-json/LICENSE.md
%doc      /usr/share/doc/tokox/cpp-json/README.md

%changelog
* Wed Mar 20 2024 Tomasz Kośnikowski (tokox) - 1.2-2
  - Updated README

* Tue Mar 19 2024 Tomasz Kośnikowski (tokox) - 1.2-1
  - Replaced std::variant with std::any
  - improved object class

* Sun Mar 10 2024 Tomasz Kośnikowski (tokox) - 1.1-1
  - Wrapped everything in tokox namespace and reformated code
  - Updated README

* Sat Mar 09 2024 Tomasz Kośnikowski (tokox) - 1.0-1
  - First Release!
