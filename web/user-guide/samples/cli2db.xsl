<?xml version="1.0" encoding="iso-8859-1"?>

<!--
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006-2007 Alexis Royer.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
-->


<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:cli="http://alexis.royer.free.fr/CLI"
        xmlns:db="http://docbook.org/docbook-ng">
	<!--xmlns:date="http://exslt.org/dates and times"-->
<xsl:output method="xml" encoding="iso-8859-1"/>


<xsl:variable name="STR_Endl"><xsl:text>
</xsl:text></xsl:variable>

<xsl:template match="/">
    <db:programlisting>
    <xsl:text>&lt;?xml version="1.0" encoding="ISO-8859-1"?&gt;</xsl:text><xsl:value-of select="$STR_Endl"/>
    <xsl:apply-templates/>
    </db:programlisting>
</xsl:template>

<xsl:template match="cli:endl">
    <xsl:call-template name="T_Indent"/>
    <xsl:text>&lt;endl</xsl:text>
        <xsl:apply-templates select="@*"/>
        <xsl:text>&gt;</xsl:text>
    <xsl:choose>
        <xsl:when test="count(cli:cpp) = 0"><xsl:apply-templates/></xsl:when>
        <xsl:when test="count(cli:cpp) = 1"><xsl:apply-templates select="cli:cpp"/></xsl:when>
        <xsl:otherwise>
            <xsl:value-of select="$STR_Endl"/>
            <xsl:for-each select="cli:cpp">
                <xsl:call-template name="T_Indent"/>
                <xsl:apply-templates select="."/>
                <xsl:value-of select="$STR_Endl"/>
            </xsl:for-each>
        </xsl:otherwise>
    </xsl:choose>
    <xsl:text>&lt;/endl&gt;</xsl:text>
        <xsl:value-of select="$STR_Endl"/>
</xsl:template>

<xsl:template match="cli:help">
    <xsl:text>&lt;help</xsl:text>
        <xsl:apply-templates select="@*"/>
        <xsl:text>&gt;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>&lt;/help&gt;</xsl:text>
</xsl:template>

<xsl:template match="cli:cpp">
    <xsl:if test="not(parent::cli:endl)">
        <xsl:call-template name="T_Indent"/>
    </xsl:if>

    <xsl:text>&lt;cpp</xsl:text>
        <xsl:apply-templates select="@*"/>
        <xsl:text>&gt;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>&lt;/cpp&gt;</xsl:text>

    <xsl:if test="not(parent::cli:endl)">
        <xsl:value-of select="$STR_Endl"/>
    </xsl:if>
</xsl:template>

<xsl:template match="cli:out">
    <xsl:text>&lt;out/&gt;</xsl:text>
</xsl:template>

<xsl:template match="cli:value-of">
    <xsl:text>&lt;value-of</xsl:text>
    <xsl:apply-templates select="@*"/>
    <xsl:text>/&gt;</xsl:text>
</xsl:template>

<xsl:template match="cli:tag[@ref]">
    <xsl:call-template name="T_Indent"/>
    <xsl:text>&lt;tag</xsl:text>
    <xsl:apply-templates select="@*"/>
    <xsl:text>/&gt;</xsl:text>
    <xsl:value-of select="$STR_Endl"/>
</xsl:template>

<xsl:template match="cli:menu[@ref]">
    <xsl:text>&lt;menu</xsl:text>
    <xsl:apply-templates select="@*"/>
    <xsl:text>&gt;</xsl:text>
</xsl:template>

<xsl:template match="cli:*">
    <xsl:if test="self::cli:menu">
        <xsl:value-of select="$STR_Endl"/>
    </xsl:if>

    <xsl:call-template name="T_Indent"/>
    <xsl:text>&lt;</xsl:text>
        <xsl:value-of select="local-name(.)"/>
        <xsl:if test=".=/">
            <xsl:text> xmlns="http://alexis.royer.free.fr/CLI"</xsl:text>
        </xsl:if>
        <xsl:apply-templates select="@*"/>
        <xsl:text>&gt;</xsl:text>
        <xsl:apply-templates select="cli:help"/>
        <xsl:value-of select="$STR_Endl"/>
    <xsl:apply-templates select="cli:*[not(self::cli:help)]"/>
    <xsl:call-template name="T_Indent"/>
        <xsl:text>&lt;/</xsl:text>
        <xsl:value-of select="local-name(.)"/>
        <xsl:text>&gt;</xsl:text>
        <xsl:value-of select="$STR_Endl"/>
</xsl:template>

<xsl:template match="@*">
    <xsl:text> </xsl:text>
    <xsl:value-of select="local-name(.)"/>
    <xsl:text>="</xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>"</xsl:text>
</xsl:template>

<xsl:template name="T_Indent">
    <xsl:if test="not(.=/)">
        <xsl:text> </xsl:text>
        <xsl:for-each select="parent::cli:*">
            <xsl:call-template name="T_Indent"/>
        </xsl:for-each>
    </xsl:if>
</xsl:template>

</xsl:stylesheet>
