/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MU_NOTATION_MASTERNOTATION_H
#define MU_NOTATION_MASTERNOTATION_H

#include <memory>

#include "../imasternotation.h"
#include "../inotationreadersregister.h"
#include "../inotationwritersregister.h"

#include "modularity/ioc.h"
#include "notation.h"
#include "retval.h"

namespace Ms {
class MasterScore;
}

namespace mu::notation {
class MasterNotation : public IMasterNotation, public Notation, public std::enable_shared_from_this<MasterNotation>
{
    INJECT(notation, INotationReadersRegister, readers)
    INJECT(notation, INotationWritersRegister, writers)

public:
    explicit MasterNotation();
    ~MasterNotation();

    INotationPtr notation() override;

    Meta metaInfo() const override;
    void setMetaInfo(const Meta& meta) override;

    Ret load(const io::path& path, const io::path& stylePath = io::path(), bool forceMode = false) override;
    io::path path() const override;

    Ret createNew(const ScoreCreateOptions& scoreOptions) override;
    RetVal<bool> created() const override;

    Ret save(const io::path& path = io::path(), SaveMode saveMode = SaveMode::Save) override;
    mu::ValNt<bool> needSave() const override;

    ValCh<ExcerptNotationList> excerpts() const override;
    void setExcerpts(const ExcerptNotationList& excerpts) override;

    INotationPartsPtr parts() const override;
    INotationPtr clone() const override;

    Ret writeToDevice(io::Device& destinationDevice) override;

private:
    Ret exportScore(const io::path& path, const std::string& suffix);

    Ms::MasterScore* masterScore() const;

    Ret load(const io::path& path, const io::path& stylePath, const INotationReaderPtr& reader, bool forceMode = false);
    Ret doLoadScore(Ms::MasterScore* score, const io::path& path, const INotationReaderPtr& reader, bool forceMode = false) const;
    mu::RetVal<Ms::MasterScore*> newScore(const ScoreCreateOptions& scoreInfo);

    void doSetExcerpts(ExcerptNotationList excerpts);

    void initExcerpts(const QList<Ms::Excerpt*>& scoreExcerpts = QList<Ms::Excerpt*>());

    void createNonexistentExcerpts(const ExcerptNotationList& newExcerpts);

    void updateExcerpts();
    IExcerptNotationPtr createExcerpt(Ms::Part* part);

    Ret saveScore(const io::path& path = io::path(), SaveMode saveMode = SaveMode::Save);
    Ret saveSelectionOnScore(const io::path& path = io::path());

    ValCh<ExcerptNotationList> m_excerpts;
};
}

#endif // MU_NOTATION_MASTERNOTATION_H
