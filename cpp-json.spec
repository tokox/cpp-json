Name:           cpp-json
Version:        1.0
Release:        1%{?dist}
Summary:        C++ JSON object import/export library

License:        MIT
URL:            https://github.com/tokox/cpp-json
Source0:        https://github.com/tokox/cpp-json/archive/v%{version}.tar.gz

BuildArch:      noarch

%description
This is a JSON object import/export library in C++. It contains class to represent JSON Object and functions to read it from stream and write it to stream. That is basically it. For more information look at README.

%prep
%setup -q

%install
mkdir -p %{buildroot}/usr/include/cpp-json
cp -a json.hpp %{buildroot}/usr/include/cpp-json
mkdir -p %{buildroot}/usr/share/doc/cpp-json
cp -a LICENSE.md README.md %{buildroot}/usr/share/doc/cpp-json

%files
/usr/include/cpp-json/json.hpp
/usr/share/doc/cpp-json/LICENSE.md
/usr/share/doc/cpp-json/README.md

%changelog
* Sat Mar 09 2024 Tomasz Kośnikowski (tokox) - 1.0-1
- First Public Release!
