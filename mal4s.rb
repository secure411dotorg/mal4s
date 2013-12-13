require 'formula'

class Mal4s < Formula

    version '0.1'

    url 'https://github.com/secure411dotorg/mal4sCore.git'
    url 'https://github.com/secure411dotorg/mal4s.git'

    depends_on :automake
    depends_on :libtool

  depends_on :x11 if MacOS::X11.installed?
  depends_on :freetype

  depends_on 'pkg-config' => :build
  depends_on 'glm' => :build

  depends_on 'boost'
  depends_on 'glew'
  depends_on 'jpeg'
  depends_on 'pcre'
  depends_on 'sdl2'
  depends_on 'sdl2_image'
  depends_on 'sdl2_mixer'

  def install
    # For non-/usr/local installs
    ENV.append "CXXFLAGS", "-I#{HOMEBREW_PREFIX}/include"
    system "autoreconf -f -i"

    system "./configure", "--disable-dependency-tracking",
                          "--prefix=#{prefix}",
                          "--without-x"
    system "make install"
  end
end

