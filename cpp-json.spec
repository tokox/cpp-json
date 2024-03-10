Name:           tokox-cpp-json
Version:        1.0
Release:        1%{?dist}
Summary:        C++ JSON object import/export library

License:        MIT
URL:            https://github.com/tokox/cpp-json
BugURL:         https://github.com/tokox/cpp-json/issues
Source0:        https://github.com/tokox/cpp-json/archive/v%{version}.tar.gz

BuildArch:      noarch

Requires:       libstdc++

%description
This is a JSON object import/export library in C++. It contains class to represent JSON Object and functions to read it from stream and write it to stream. That is basically it. For more information look at README.

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
* Sat Mar 09 2024 Tomasz Kośnikowski (tokox) - 1.0-1
- First Release!
