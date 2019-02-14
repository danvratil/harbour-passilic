Name:       harbour-passilic

%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary:    A frontend for the Pass password manager
Version:    0.2
Release:    1
Group:      Applications/Productivity
License:    GPLv3 and LGPLv2+
URL:        https://github.com/danvratil/harbour-passilic
Source0:    %{name}-%{version}.tar.bz2


Requires:   sailfishsilica-qt5 >= 0.10.9
# gnupg2 is not an allowed dependency on Harbour, but it should be
# already pre-installed on SFOS, so no harm commenting it out here.
#Requires:   gnupg2

BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
A frontend for the Pass password manager. Pass is a simple Unix password
manager where each password is stored GPG encrypted in a file. Passilic
allows you to easily browse the passwords. Tapping a passwords prompts
the "master key" (your GPG key passphrase) and copies the password into
clipboard. The password is automatically cleared from the clipboard
after 45 seconds.


%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5 DEFINES+=PASSILIC_VERSION=\\\\\\\"\"%{version}\\\\\\\"\"
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original \
  --dir %{buildroot}%{_datadir}/applications \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%defattr(0666,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png

%changelog
* Thu Feb 14 2019 Daniel Vrátil <dvratil@kde.org> 0.2-1
- Clear passwords on exit
- Automatically focus passphrase entry field
- Add Search page
- Make password timeout customizable

* Mon Feb 04 2019 Daniel Vrátil <dvratil@kde.org> 0.1-1
- Initial release
